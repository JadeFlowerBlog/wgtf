#include "command_instance.hpp"
#include "command.hpp"
#include "i_command_manager.hpp"

#include "core_data_model/collection_model.hpp"

#include "core_reflection/generic/generic_object.hpp"
#include "core_reflection/interfaces/i_class_definition.hpp"
#include "core_reflection/i_definition_manager.hpp"
#include "core_reflection/i_object_manager.hpp"
#include "core_reflection/metadata/meta_impl.hpp"
#include "core_reflection/metadata/meta_utilities.hpp"
#include "core_reflection/property_accessor_listener.hpp"
#include "core_reflection/property_accessor.hpp"
#include "core_reflection/property_iterator.hpp"
#include "core_reflection/interfaces/i_base_property.hpp"
#include "core_serialization/resizing_memory_stream.hpp"
#include "core_serialization/serializer/i_serialization_manager.hpp"

#include "core_logging/logging.hpp"

namespace RPURU = ReflectedPropertyUndoRedoUtility;
namespace
{

	//==========================================================================
	class PropertyAccessorWrapper
		: public PropertyAccessorListener
	{
	public:
		PropertyAccessorWrapper(
			RPURU::UndoRedoHelperList & undoRedoHelperList,
			const std::thread::id& commandThreadId )
			: undoRedoHelperList_( undoRedoHelperList ) 
			, commandThreadId_( commandThreadId )
		{
		}

		//======================================================================
		void preSetValue(
			const PropertyAccessor & accessor, const Variant & value ) override
		{
			assert( (std::this_thread::get_id() == commandThreadId_) &&
				"To record undo/redo data, properties must be changed in a"
				"command on the command thread" );

			const auto & obj = accessor.getRootObject();
			assert( obj != nullptr );
			RefObjectId id;
			bool ok = obj.getId( id );
			assert( ok );
			const char * propertyPath = accessor.getFullPath();
			const TypeId type = accessor.getType();
			Variant prevalue = accessor.getValue();
			auto pHelper = this->findUndoRedoHelper( id, propertyPath, type );
			if (pHelper != nullptr)
			{
				return;
			}
			RPURU::ReflectionPropertyUndoRedoHelper helper;
			helper.objectId_ = id;
			helper.propertyPath_ = propertyPath;
			helper.propertyTypeName_ = type.getName();
			helper.preValue_ = std::move( prevalue );
			undoRedoHelperList_.emplace_back( helper );
		}


		//======================================================================
		void postSetValue(
			const PropertyAccessor & accessor, const Variant & value ) override
		{
			assert( (std::this_thread::get_id() == commandThreadId_) &&
				"To record undo/redo data, properties must be changed in a"
				"command on the command thread" );

			const auto & obj = accessor.getRootObject();
			assert( obj != nullptr );
			RefObjectId id;
			bool ok = obj.getId( id );
			assert( ok );
			const char * propertyPath = accessor.getFullPath();
			const TypeId type = accessor.getType();
			 Variant postValue = accessor.getValue();
			auto pHelper = this->findUndoRedoHelper( id, propertyPath, type );
			assert( pHelper != nullptr );
			pHelper->postValue_ = std::move( postValue );
		}

	private:
		RPURU::ReflectionPropertyUndoRedoHelper* findUndoRedoHelper( 
			const RefObjectId & id, const char * propertyPath, const TypeId & type )
		{
			RPURU::ReflectionPropertyUndoRedoHelper* helper = nullptr;
			for (auto& findIt : undoRedoHelperList_)
			{
				if ((findIt.objectId_ == id) && 
					(findIt.propertyPath_ == propertyPath) && 
					(findIt.propertyTypeName_ == type.getName()))
				{
					helper = &findIt;
					break;
				}
			}
			return helper;
		}
	private:
		RPURU::UndoRedoHelperList &	undoRedoHelperList_;
		const std::thread::id& commandThreadId_;
	};

}

//==============================================================================
CommandInstance::CommandInstance()
	: defManager_( nullptr )
	, status_( Complete )
	, arguments_( nullptr )
	, pCmdSysProvider_( nullptr )
	, commandId_("")
	, contextObject_( nullptr )
	, errorCode_( CommandErrorCode::NO_ERROR )
{
}

//==============================================================================
CommandInstance::CommandInstance( const CommandInstance& )
{
	assert(!"Not copyable");
}

//==============================================================================
/*virtual */void CommandInstance::init( const std::thread::id& commandThreadId )
{
	paListener_ = std::make_shared< PropertyAccessorWrapper >(
		undoRedoHelperList_,
		commandThreadId );

	const char * undoStreamHeaderTag = RPURU::getUndoStreamHeaderTag();
	const char * redoStreamHeaderTag = RPURU::getRedoStreamHeaderTag();

	undoData_.seek( 0 );
	undoData_.write( undoStreamHeaderTag );

	redoData_.seek( 0 );
	redoData_.write( redoStreamHeaderTag );
}


//==============================================================================
CommandInstance::~CommandInstance()
{
	assert( undoRedoHelperList_.empty() );
	paListener_ = nullptr;
}


//==============================================================================
void CommandInstance::cancel()
{
}


//==============================================================================
void CommandInstance::waitForCompletion()
{
	std::unique_lock<std::mutex> lock( mutex_ );

	while( !completeStatus_.wait_for(
		lock,
		std::chrono::milliseconds( 1 ),
		[this] { return status_ == Complete; } ) )
	{
		lock.unlock();
		getCommand()->fireProgressMade( *this );
		lock.lock();
	}
}


//==============================================================================
CommandErrorCode CommandInstance::getErrorCode() const
{
	if (children_.empty())
	{
		return errorCode_;
	}

	//batchcommand(parent command) failure only when:
	//all sub-commands failed(errorcode != NO_ERROR) or as long as one of sub-commands' errorcode == ABORTED
	CommandErrorCode errorCode = CommandErrorCode::ABORTED;
	for (const auto & child : children_)
	{
		CommandErrorCode childErrorCode = child->getErrorCode();
		if (childErrorCode == CommandErrorCode::NO_ERROR)
		{
			errorCode = childErrorCode;
			continue;
		}
		if (childErrorCode == CommandErrorCode::ABORTED)
		{
			errorCode = childErrorCode;
			break;
		}
	}
	return errorCode;
}

//==============================================================================
bool CommandInstance::isMultiCommand() const
{
	return !children_.empty();
}

//==============================================================================
void CommandInstance::setArguments( const ObjectHandle & arguments )
{
	arguments_ = arguments;
}

//==============================================================================
void CommandInstance::setDefinitionManager( IDefinitionManager & defManager )
{
	defManager_ = &defManager;
}

//==============================================================================
const char * CommandInstance::getCommandId() const
{
	return commandId_.c_str();
}


//==============================================================================
void CommandInstance::setCommandId( const char * commandName )
{
	commandId_ = commandName;
}

//==============================================================================
Command * CommandInstance::getCommand()
{
	assert( pCmdSysProvider_ != nullptr );
	Command * pCommand = pCmdSysProvider_->findCommand( commandId_.c_str() );
	assert( pCommand != nullptr );
	return pCommand;
}


const Command * CommandInstance::getCommand() const
{
	assert( pCmdSysProvider_ != nullptr );
	Command * pCommand = pCmdSysProvider_->findCommand( commandId_.c_str() );
	assert( pCommand != nullptr );
	return pCommand;
}

//==============================================================================
/*virtual */void CommandInstance::setStatus( ExecutionStatus status )
{
	std::unique_lock<std::mutex> lock( mutex_ );

	status_ = status;
	getCommand()->fireCommandStatusChanged( *this );
	if (status_ == Complete)
	{
		completeStatus_.notify_all();
	}
}

//==============================================================================
void CommandInstance::undo()
{
	assert( defManager_ != nullptr );
	const auto pObjectManager = defManager_->getObjectManager();
	assert( pObjectManager != nullptr );
	const bool result = 
		RPURU::performReflectedUndo( undoData_, (*pObjectManager) );
	if (result)
	{
		getCommand()->undo( undoData_ );
	}
}


//==============================================================================
void CommandInstance::redo()
{
	assert( defManager_ != nullptr );
	const auto pObjectManager = defManager_->getObjectManager();
	assert( pObjectManager != nullptr );
	const bool result = 
		RPURU::performReflectedRedo( redoData_, (*pObjectManager) );
	if (result)
	{
		getCommand()->redo( redoData_ );
	}
}


//==============================================================================
void CommandInstance::execute()
{
	returnValue_ = getCommand()->execute( arguments_ );
	auto errorCode = returnValue_.getBase<CommandErrorCode>();
	if (errorCode != nullptr)
	{
		errorCode_ = *errorCode;
	}
}

//==============================================================================
void CommandInstance::connectEvent()
{
	assert( paListener_ );
	assert( defManager_ != nullptr );
	defManager_->registerPropertyAccessorListener( paListener_ );
}

//==============================================================================
void CommandInstance::disconnectEvent()
{
	assert( paListener_ );
	assert( defManager_ != nullptr );
	defManager_->deregisterPropertyAccessorListener( paListener_ );
	IObjectManager* objManager = defManager_->getObjectManager();
	assert( objManager != nullptr );
	ISerializationManager * serializationMgr = objManager->getSerializationManager();
	assert( serializationMgr != nullptr );
	for (const auto& helper : undoRedoHelperList_)
	{
		RPURU::saveUndoData( *serializationMgr, undoData_, helper );
		RPURU::saveRedoData( *serializationMgr, redoData_, helper );
	}
	undoRedoHelperList_.clear();
}

//==============================================================================
void CommandInstance::getUndoData(std::string * undoData) const
{
	undoData->assign( static_cast<const char *>(undoData_.rawBuffer()), undoData_.size());
}

//==============================================================================
void CommandInstance::setUndoData( const std::string & undoData )
{
	undoData_.resetData();
	size_t size = undoData_.writeRaw( undoData.data(), undoData.length() );
	assert(size == undoData.length());
}

//==============================================================================
void CommandInstance::getRedoData(std::string * redoData) const
{
	redoData->assign( static_cast<const char *>(redoData_.rawBuffer()), redoData_.size());
}

//==============================================================================
void CommandInstance::setRedoData( const std::string & redoData )
{
	redoData_.resetData();
	size_t size = redoData_.writeRaw( redoData.data(), redoData.length() );
	assert(size == redoData.length());
}

//==============================================================================
void CommandInstance::setContextObject( const ObjectHandle & contextObject )
{
	contextObject_ = contextObject;
}

//==============================================================================
void CommandInstance::setCommandSystemProvider( ICommandManager * pCmdSysProvider )
{
	pCmdSysProvider_ = pCmdSysProvider;
}