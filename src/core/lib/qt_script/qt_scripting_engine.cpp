#include "qt_scripting_engine.hpp"

#include "qobject_qt_type_converter.hpp"
#include "qt_script_object.hpp"
#include "qt_common/i_qt_framework.hpp"
#include "qt_common/controls/bw_copyable.hpp"
#include "script_qt_type_converter.hpp"
#include "wg_list_iterator.hpp"
#include "collection_qt_type_converter.hpp"

#include "reflection/base_property.hpp"
#include "reflection/class_definition.hpp"
#include "reflection/definition_manager.hpp"
#include "reflection/reflected_object.hpp"

#include "command_system/i_command_manager.hpp"

#include "generic_plugin/interfaces/i_context_manager.hpp"

#include "copy_paste_system/i_copy_paste_manager.hpp"

#include <private/qmetaobjectbuilder_p.h>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlContext>

Q_DECLARE_METATYPE( ObjectHandle );

QtScriptingEngine::QtScriptingEngine()
	: defManager_( nullptr )
	, commandSystemProvider_( nullptr )
	, copyPasteManager_( nullptr )
	, contextManager_( nullptr )
{
}

QtScriptingEngine::~QtScriptingEngine()
{
	assert( scriptObjects_.empty() );

	std::lock_guard< std::mutex > guard( metaObjectsMutex_ );

	for (auto& metaObjectPair: metaObjects_)
	{
		free( metaObjectPair.second );
	}

	metaObjects_.clear();
}

void QtScriptingEngine::initialise( 
	IQtFramework & qtFramework, IContextManager & contextManager )
{	
	contextManager_ = &contextManager;
	defManager_ = contextManager.queryInterface< IDefinitionManager >();
	commandSystemProvider_ =
		contextManager.queryInterface< ICommandManager >();

	copyPasteManager_ = 
		contextManager.queryInterface<ICopyPasteManager>();
	assert( defManager_ );
	assert( commandSystemProvider_ );
	assert( copyPasteManager_ );


	qtTypeConverters_.emplace_back( new GenericQtTypeConverter< ObjectHandle >() );
	qtTypeConverters_.emplace_back( new CollectionQtTypeConverter() );
	qtTypeConverters_.emplace_back( new QObjectQtTypeConverter() );
	qtTypeConverters_.emplace_back( new ScriptQtTypeConverter( *this ) );

	QMetaType::registerComparators< ObjectHandle >();
	for (auto & qtTypeConverter : qtTypeConverters_)
	{
		qtFramework.registerTypeConverter( *qtTypeConverter );
	}
}

void QtScriptingEngine::finalise()
{
	for (auto& scriptObject: scriptObjects_)
	{
		delete scriptObject.second;
	}

	scriptObjects_.clear();
}

QtScriptObject * QtScriptingEngine::createScriptObject( 
	const ObjectHandle & object )
{
	auto itr = scriptObjects_.find( object );

	if (itr != scriptObjects_.end())
	{
		return itr->second;
	}

	auto classDefinition = object.getDefinition();
	if (classDefinition == nullptr)
	{
		return nullptr;
	}

	int firstMethodIndex;
	auto metaObject = getMetaObject( *classDefinition, firstMethodIndex );
	if (metaObject == nullptr)
	{
		return nullptr;
	}

	assert( contextManager_ );
	QtScriptObject* scriptObject = new QtScriptObject(
		*contextManager_, *metaObject, object, firstMethodIndex, nullptr );

	scriptObjects_.emplace( object, scriptObject );
	return scriptObject;
}

QObject * QtScriptingEngine::createObject( QString definition )
{
	auto className = std::string( "class " ) + definition.toUtf8().constData();

	if (defManager_ == nullptr)
	{
		qCritical( "Definition manager not found. Could not create object: %s \n", 
			className.c_str() );
		return nullptr;
	}

	auto classDefinition = defManager_->getDefinition( className.c_str() );
	if (classDefinition == nullptr)
	{
		qWarning( "No definition registered for type: %s \n", className.c_str() );
		return nullptr;
	}

	auto object = classDefinition->createManagedObject();
	if (object == nullptr)
	{
		qWarning( "Could not create C++ type: %s \n", className.c_str() );
		return nullptr;
	}

	// no parent as qml takes ownership of this object
	auto scriptObject = createScriptObject( object );
	if (scriptObject == nullptr)
	{
		qWarning( "Could not create Qt type: %s \n", className.c_str() );
		return nullptr;
	}

	return scriptObject;
}

QVariant QtScriptingEngine::executeCommand( QString command )
{
	auto commandId = std::string( "class " ) + command.toUtf8().constData();
	Command * cmd = commandSystemProvider_->findCommand( commandId.c_str() );
	if(cmd == nullptr)
	{
		qWarning( "Could not find Command: %s \n", commandId.c_str() );
		return QVariant();
	}
	commandSystemProvider_->executeCommand( commandId.c_str() );
	return QVariant();
}

bool QtScriptingEngine::queueCommand( QString command )
{
	auto commandId = std::string( "class " ) + command.toUtf8().constData();
	Command * cmd = commandSystemProvider_->findCommand( commandId.c_str() );
	if(cmd == nullptr)
	{
		qWarning( "Could not find Command: %s \n", commandId.c_str() );
		return false;
	}
	commandSystemProvider_->queueCommand( commandId.c_str() );
	return true;
}

void QtScriptingEngine::beginUndoFrame()
{
	commandSystemProvider_->beginBatchCommand();
}

void QtScriptingEngine::endUndoFrame()
{
	commandSystemProvider_->endBatchCommand();
}

void QtScriptingEngine::abortUndoFrame()
{
	commandSystemProvider_->abortBatchCommand();
}

void QtScriptingEngine::deleteMacro( QString command )
{
	std::string commandId = command.toUtf8().constData();
	Command * cmd = commandSystemProvider_->findCommand( commandId.c_str() );
	if(cmd == nullptr)
	{
		qWarning( "Delete macro failed: Could not find Macro: %s \n", commandId.c_str() );
		return;
	}
	commandSystemProvider_->deleteCompoundCommand( commandId.c_str() );
}

void QtScriptingEngine::selectControl( BWCopyable* control, bool append )
{
	copyPasteManager_->onSelect( control, append );
}

void QtScriptingEngine::deselectControl( BWCopyable* control, bool reset )
{
	copyPasteManager_->onDeselect( control, reset );
}

QObject * QtScriptingEngine::iterator( const QVariant & collection )
{
	int typeId = collection.type();
	if (typeId == QVariant::UserType)
	{
		typeId = collection.userType();
	}

	if (typeId != qMetaTypeId< ObjectHandle >())
	{
		return nullptr;
	}

	auto handle = collection.value< ObjectHandle >();
	if (!handle.isValid())
	{
		return nullptr;
	}

	auto listModel = handle.getBase< IListModel >();
	if (listModel == nullptr)
	{
		return nullptr;
	}

	// QML will take ownership of this object
	return new WGListIterator( *listModel );
}

QMetaObject * QtScriptingEngine::getMetaObject(
	const IClassDefinition & classDefinition, int& firstMethodIndex )
{
	auto definition = classDefinition.getName();

	{
		std::lock_guard< std::mutex > guard( metaObjectsMutex_ );
		auto metaObjectIt = metaObjects_.find( definition );
		if ( metaObjectIt != metaObjects_.end() )
		{
			QMetaObject* object = metaObjectIt->second;
			firstMethodIndex =
				object->indexOfMethod( "getMetaObject(QString)" ) -
				object->methodOffset();
			return object;
		}
	}

	QMetaObjectBuilder builder;
	builder.setClassName( definition );
	builder.setSuperClass( &QObject::staticMetaObject );

	auto thisProperty = builder.addProperty( "self", "QObject*" );
	thisProperty.setWritable( false );
	thisProperty.setConstant( true );

	// Add all the properties from the ClassDefinition to the builder
	auto properties = classDefinition.allProperties();
	auto it = properties.begin();
	for (; it != properties.end(); ++it)
	{
		auto property = builder.addProperty( it->getName(), "QVariant" );
		property.setWritable( true );

		auto notifySignal = std::string( it->getName() ) + "Changed(QVariant)";
		property.setNotifySignal( builder.addSignal( notifySignal.c_str() ) );
	}

	//TODO: Move these to actual methods on the scripting engine.
	firstMethodIndex =
		builder.addMethod( "getMetaObject(QString)", "QVariant" ).index();
	builder.addMethod( "getMetaObject(QString,QString)", "QVariant" );
	builder.addMethod( "containsMetaType(QString,QString)", "QVariant" );

	auto metaObject = builder.toMetaObject();
	if (metaObject == nullptr)
	{
		return nullptr;
	}

	{
		std::lock_guard< std::mutex > guard( metaObjectsMutex_ );
		auto inserted = metaObjects_.insert( 
			std::pair< std::string, QMetaObject * >( definition, metaObject ) );
		if (!inserted.second)
		{
			free( metaObject );
		}
		return inserted.first->second;
	}
}