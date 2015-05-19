#include "pch.hpp"
#include "test_command_system_fixture.hpp"
#include "reflection/definition_manager.hpp"
#include "reflection/object_manager.hpp"
#include "reflection/reflected_types.hpp"
#include "reflection/variant_handler.hpp"
#include "command_system/command_manager.hpp"
#include "serialization/serializer/serialization_manager.hpp"
#include "reflection_utils/reflected_types.hpp"
#include "reflection_utils/commands/set_reflectedproperty_command.hpp"
#include "reflection_utils/command_system_property_setter.hpp"
#include "reflection_utils/serializer/reflection_serializer.hpp"
#include "command_system/command_system.hpp"

//==============================================================================
TestCommandSystemFixture::TestCommandSystemFixture()
	: objectManager_( new ObjectManager() )
	, definitionManager_( new DefinitionManager( *objectManager_ ) )
	, commandManager_( new CommandManager( *definitionManager_ ) )
	, serializationManager_( new SerializationManager() )
	, setReflectedPropertyCmd_( new SetReflectedPropertyCommand() )
	, commandSystemReflectionPropertySetter_( new CommandSystemReflectionPropertySetter() )
	, multiCommandStatus_( MultiCommandStatus_Begin )
{
	objectManager_->init( definitionManager_.get() );
	objectManager_->setSerializationManager( serializationManager_.get() );
	Reflection::initReflectedTypes( *definitionManager_ );
	Reflection_Utils::initReflectedTypes( *definitionManager_ );
	CommandSystem::initReflectedTypes( *definitionManager_ );
	auto metaTypeMgr = Variant::getMetaTypeManager();

	auto metaType = metaTypeMgr->findType< ObjectHandle >();
	variantStorageLookupHandler_.reset( new ReflectionStorageLookupHandler(
		definitionManager_.get(),
		metaType ) );

	metaTypeMgr->registerDynamicStorageHandler(
		*variantStorageLookupHandler_ );

	reflectionSerializer_.reset( 
		new ReflectionSerializer( 
		*serializationManager_, *metaTypeMgr, *objectManager_ ) );
	objectManager_->setSerializationManager( serializationManager_.get() );
	for(auto type : reflectionSerializer_->getSupportedType())
	{
		serializationManager_->registerSerializer( 
			type.getName(), reflectionSerializer_.get() );
	}
	commandManager_->init();
	commandManager_->registerCommand( setReflectedPropertyCmd_.get() );

	commandSystemReflectionPropertySetter_->init( *commandManager_ );
	commandManager_->registerCommandStatusListener( this );
}


//==============================================================================
TestCommandSystemFixture::~TestCommandSystemFixture()
{
	commandManager_->deregisterCommand( setReflectedPropertyCmd_->getId() );
	commandManager_->fini();

	for(auto type : reflectionSerializer_->getSupportedType())
	{
		serializationManager_->deregisterSerializer( type.getName() );
	}
	
	auto metaTypeMgr = Variant::getMetaTypeManager();
	metaTypeMgr->deregisterDynamicStorageHandler(
		*variantStorageLookupHandler_ );
	setReflectedPropertyCmd_.reset();
	reflectionSerializer_.reset();
	serializationManager_.reset();
	commandManager_.reset();
	objectManager_.reset(); 
	definitionManager_.reset();
}


//==============================================================================
IObjectManager & TestCommandSystemFixture::getObjectManager() const
{
	return *objectManager_;
}


//==============================================================================
IDefinitionManager & TestCommandSystemFixture::getDefinitionManager() const
{
	return *definitionManager_;
}


//==============================================================================
CommandSystemProvider & TestCommandSystemFixture::getCommandSystemProvider() const
{
	return *commandManager_;
}


//==============================================================================
IReflectionPropertySetter & TestCommandSystemFixture::getReflectionPropertySetter() const
{
	return *commandSystemReflectionPropertySetter_;
}


//==============================================================================
const ICommandEventListener::MultiCommandStatus & TestCommandSystemFixture::getMultiCommandStatus() const
{
	return multiCommandStatus_;
}


//==============================================================================
void TestCommandSystemFixture::multiCommandStatusChanged( 
	ICommandEventListener::MultiCommandStatus multiCommandStatus ) const
{
	multiCommandStatus_ = multiCommandStatus;
}
