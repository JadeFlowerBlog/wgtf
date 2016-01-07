#include "core_generic_plugin/generic_plugin.hpp"
#include "core_variant/variant.hpp"
#include "balance_panel.hpp"

#include <memory>


struct Python27TestUIPlugin
	: public PluginMain
{
	Python27TestUIPlugin( IComponentContext& componentContext )
	{
	}


	bool PostLoad( IComponentContext& componentContext ) override
	{
		return true;
	}


	void Initialise( IComponentContext& componentContext ) override
	{
		// Initialise variant system; this is required for every plugin that uses Variant.
		auto metaTypeManager = componentContext.queryInterface<IMetaTypeManager>();
		Variant::setMetaTypeManager( metaTypeManager );
		balancePanel_.reset( new BalancePanel( componentContext ) );
	}


	bool Finalise( IComponentContext& componentContext ) override
	{
		balancePanel_.reset();
		return true;
	}


	void Unload( IComponentContext& componentContext ) override
	{
	}


	std::unique_ptr< BalancePanel > balancePanel_;
};


PLG_CALLBACK_FUNC( Python27TestUIPlugin )
