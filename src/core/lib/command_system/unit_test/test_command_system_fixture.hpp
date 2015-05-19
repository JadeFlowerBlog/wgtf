#ifndef TEST_COMMAND_SYSTEM_FIXTURE_HPP
#define TEST_COMMAND_SYSTEM_FIXTURE_HPP

#include <memory>
#include "command_system/i_command_event_listener.hpp"

class IStorageLookupHandler;
class ObjectManager;
class CommandManager;
class ISerializationManager;
class CommandSystemProvider;
class CommandSystemReflectionPropertySetter;
class Command;
class ReflectionSerializer;
class IDefinitionManager;
class IObjectManager;
class IReflectionPropertySetter;

class TestCommandSystemFixture
	: public ICommandEventListener
{
public:
	TestCommandSystemFixture();
	~TestCommandSystemFixture();
	IObjectManager & getObjectManager() const;
	IDefinitionManager & getDefinitionManager() const;
	CommandSystemProvider & getCommandSystemProvider() const;
	IReflectionPropertySetter & getReflectionPropertySetter() const;
	const ICommandEventListener::MultiCommandStatus & getMultiCommandStatus() const;
	void multiCommandStatusChanged( ICommandEventListener::MultiCommandStatus multiCommandStatus ) const override;

private:
	std::unique_ptr< ObjectManager > objectManager_;
	std::unique_ptr< IDefinitionManager > definitionManager_;
	std::unique_ptr< IStorageLookupHandler > variantStorageLookupHandler_;
	std::unique_ptr< CommandManager > commandManager_;
	std::unique_ptr< ISerializationManager > serializationManager_;
	std::unique_ptr< Command > setReflectedPropertyCmd_;
	std::unique_ptr< ReflectionSerializer > reflectionSerializer_;
	std::unique_ptr< CommandSystemReflectionPropertySetter > commandSystemReflectionPropertySetter_;
	mutable ICommandEventListener::MultiCommandStatus multiCommandStatus_;

};
#endif //TEST_COMMAND_SYSTEM_FIXTURE_HPP
