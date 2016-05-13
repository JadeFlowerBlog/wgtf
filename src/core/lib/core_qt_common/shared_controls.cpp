#include "shared_controls.hpp"

#include "controls/bw_checkbox.hpp"
#include "controls/bw_combobox.hpp"
#include "controls/bw_filedialog_qi.hpp"
#include "controls/bw_textfield.hpp"
#include "controls/popup_window.hpp"
#include "controls/bw_copyable.hpp"
#include "controls/wg_context_menu.hpp"
#include "controls/wg_action.hpp"
#include "helpers/wg_filtered_list_model.hpp"
#include "helpers/wg_filtered_tree_model.hpp"
#include "helpers/wg_filter.hpp"
#include "helpers/wg_string_filter.hpp"
#include "helpers/wg_tokenized_string_filter.hpp"
#include "models/adapters/sequence_list_adapter.hpp"
#include "models/data_change_notifier.hpp"
#include "models/extensions/asset_item_extension.hpp"
#include "models/extensions/column_extension.hpp"
#include "models/extensions/component_extension.hpp"
#include "models/extensions/selection_extension.hpp"
#include "models/extensions/thumbnail_extension.hpp"
#include "models/extensions/tree_extension.hpp"
#include "models/extensions/value_extension.hpp"
#include "models/extensions/buttons_definition_extension.hpp"
#include "models/extensions/buttons_model_extension.hpp"
#include "models/wg_list_model.hpp"
#include "models/wg_tree_model.hpp"
#include "models/wg_tree_list_adapter.hpp"
#include "helpers/selection_helper.hpp"
#include "core_data_model/asset_browser/i_asset_browser_model.hpp"
#include "core_data_model/asset_browser/i_asset_browser_model.mpp"
#include "core_data_model/asset_browser/i_asset_browser_view_model.hpp"
#include "core_data_model/asset_browser/i_asset_browser_view_model.mpp"
#include "core_data_model/asset_browser/i_asset_browser_event_model.hpp"
#include "core_data_model/asset_browser/i_asset_browser_event_model.mpp"
#include "core_data_model/asset_browser/asset_browser_breadcrumbs_model.hpp"
#include "core_data_model/i_active_filters_model.hpp"
#include "core_data_model/i_active_filters_model.mpp"
#include "core_data_model/i_breadcrumbs_model.hpp"
#include "core_data_model/i_breadcrumbs_model.mpp"
#include <QtQuick>

//==============================================================================
/*static */void SharedControls::init()
{
	qmlRegisterType< BWComboBox, 1 >("BWControls", 1, 0, "BWComboBox" );
	qmlRegisterType< PopupWindow, 1 >("BWControls", 1, 0, "PopupWindow" );
	qmlRegisterType< BWTextField, 1 >("BWControls", 1, 0, "BWTextField" );
	qmlRegisterType< BWCheckBox, 1 >("BWControls", 1, 0, "BWCheckBox" );
	qmlRegisterType< SequenceListAdapter, 1 >("BWControls", 1, 0, "SequenceList" );
	qmlRegisterType< BWFileDialogQI, 1 >( "BWControls", 1, 0, "BWFileDialogQI" );
	qmlRegisterType< BWCopyable, 1 >( "BWControls", 1, 0, "BWCopyable" );
	qmlRegisterType< WGListModel, 1 >( "WGControls", 1, 0, "WGListModel" );
	qmlRegisterType< DataChangeNotifier, 1 >( "BWControls", 1, 0, "BWDataChangeNotifier" );
	qmlRegisterType< WGTreeModel, 1 >( "WGControls", 1, 0, "WGTreeModel" );
	qmlRegisterType< WGTreeListAdapter, 1 >( "WGControls", 1, 0, "WGTreeListAdapter" );
	qmlRegisterType< ColumnExtension, 1 >( "WGControls", 1, 0, "ColumnExtension" );
	qmlRegisterType< ComponentExtension, 1 >( "WGControls", 1, 0, "ComponentExtension" );
	qmlRegisterType< SelectionExtension, 1 >( "WGControls", 1, 0, "SelectionExtension" );
	qmlRegisterType< ThumbnailExtension, 1 >( "WGControls", 1, 0, "ThumbnailExtension" );
	qmlRegisterType< TreeExtension, 1 >( "WGControls", 1, 0, "TreeExtension" );
	qmlRegisterType< ValueExtension, 1 >( "WGControls", 1, 0, "ValueExtension" );
	qmlRegisterType< AssetItemExtension, 1 >( "WGControls", 1, 0, "AssetItemExtension" );
	qmlRegisterType< WGFilteredTreeModel, 1 >( "WGControls", 1, 0, "WGFilteredTreeModel" );
	qmlRegisterType< WGFilteredListModel, 1 >( "WGControls", 1, 0, "WGFilteredListModel" );
	qmlRegisterType< SelectionHelper, 1 >( "WGControls", 1, 0, "SelectionHelper" );
	qmlRegisterType< WGContextMenu, 1 >( "WGControls", 1, 0, "WGContextMenu" );
	qmlRegisterType< WGAction, 1 >( "WGControls", 1, 0, "WGAction" );

	// Filters
	qmlRegisterType< WGFilter, 1 >( "WGControls", 1, 0, "WGFilter" );
	qmlRegisterType< WGStringFilter, 1 >( "WGControls", 1, 0, "WGStringFilter" );
	qmlRegisterType< WGTokenizedStringFilter, 1 >( "WGControls", 1, 0, "WGTokenizedStringFilter" );

    qmlRegisterType< ButtonsDefinitionExtension, 1 >("WGControls", 1, 0, "ButtonsDefinitionExtension");
    qmlRegisterType< ButtonsModelExtension, 1 >("WGControls", 1, 0, "ButtonsModelExtension");
}


//==============================================================================
/*static */void SharedControls::initDefs(
	IDefinitionManager & definitionManager )
{
	definitionManager.registerDefinition( new TypeClassDefinition< IAssetBrowserModel >() );
	definitionManager.registerDefinition( new TypeClassDefinition< IAssetBrowserViewModel >() );
	definitionManager.registerDefinition( new TypeClassDefinition< IAssetBrowserEventModel >() );

	definitionManager.registerDefinition( new TypeClassDefinition< IActiveFiltersModel >() );
	definitionManager.registerDefinition( new TypeClassDefinition< ActiveFilterTerm >() );
	definitionManager.registerDefinition( new TypeClassDefinition< SavedActiveFilter >() );

	definitionManager.registerDefinition( new TypeClassDefinition< IBreadcrumbsModel >() );
	definitionManager.registerDefinition( new TypeClassDefinition< BaseBreadcrumbItem >() );
}
