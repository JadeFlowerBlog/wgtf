#include "generic_plugin/generic_plugin.hpp"
#include "generic_plugin_manager/generic_plugin_manager.hpp"
#include "reflection/i_definition_manager.hpp"
#include "reflection/type_class_definition.hpp"
#include "variant/variant.hpp"
#include "asset_browser_manager.hpp"
#include "models/asset_browser_page_model.hpp"
#include "models/folder_content_object_model.hpp"
#include <vector>

class AssetBrowserPlugin
	: public PluginMain
{
public:

	AssetBrowserPlugin( IContextManager & contextManager )
		: assetBrowserManager_( nullptr )
	{
	}

	bool PostLoad( IContextManager & contextManager ) override
	{		
		auto definitionManager = 
			contextManager.queryInterface<IDefinitionManager>();
		assert( definitionManager != nullptr );
		
		definitionManager->registerDefinition(
			new TypeClassDefinition<FolderContentObjectModel>() );
		
		assetBrowserManager_ = new AssetBrowserManager( contextManager );

		types_.push_back( contextManager.registerInterface( 
			assetBrowserManager_, false ) );

		return true;
	}

	void Initialise( IContextManager & contextManager ) override
	{		
 		Variant::setMetaTypeManager( 
 			contextManager.queryInterface< IMetaTypeManager >() );

		assetBrowserManager_->initialise( contextManager );
	}

	bool Finalise( IContextManager & contextManager ) override
	{
		return true;
	}

	void Unload( IContextManager & contextManager ) override
	{
		for ( auto type: types_ )
		{
			contextManager.deregisterInterface( type );
		}

		if (assetBrowserManager_ != nullptr)
		{
			delete assetBrowserManager_;
			assetBrowserManager_ = nullptr;
		}
	}

private:

	std::vector< IInterface * > types_;
	AssetBrowserManager* assetBrowserManager_;
};

PLG_CALLBACK_FUNC( AssetBrowserPlugin )
