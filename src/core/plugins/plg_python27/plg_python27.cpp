#include "core_python27/pch.hpp"
#include "core_generic_plugin/generic_plugin.hpp"
#include "core_reflection/i_definition_manager.hpp"
#include "core_reflection/i_object_manager.hpp"
#include "core_reflection/reflection_macros.hpp"
#include "core_python27/defined_instance.hpp"
#include "core_python27/scenario.hpp"
#include "core_python27/scripting_engine.hpp"


/**
 *	Controls initialization and finalization ong Python and
 *	registers the Python interface to be used by other plugins.
 */
class Python27Plugin
	: public PluginMain
{
public:
	Python27Plugin( IComponentContext & contextManager )
		: pInterface_( nullptr )
		, interpreter_( contextManager )
	{
	}


	bool PostLoad( IComponentContext & contextManager ) override
	{
		const bool transferOwnership = false;
		pInterface_ = contextManager.registerInterface(
			&interpreter_, transferOwnership );

		return true;
	}


	void Initialise( IComponentContext & contextManager ) override
	{
		Variant::setMetaTypeManager(
			contextManager.queryInterface< IMetaTypeManager >() );

		auto pDefinitionManager_ =
			contextManager.queryInterface< IDefinitionManager >();
		if (pDefinitionManager_ == nullptr)
		{
			return;
		}

		IDefinitionManager& definitionManager = (*pDefinitionManager_);
		REGISTER_DEFINITION( ReflectedPython::DefinedInstance );
		REGISTER_DEFINITION( Scenario );

		interpreter_.init();
	}


	bool Finalise( IComponentContext & contextManager ) override
	{
		interpreter_.fini();
		return true;
	}


	void Unload( IComponentContext & contextManager )
	{
		contextManager.deregisterInterface( pInterface_ );
	}

private:
	IInterface * pInterface_;
	Python27ScriptingEngine interpreter_;
};

PLG_CALLBACK_FUNC( Python27Plugin )

