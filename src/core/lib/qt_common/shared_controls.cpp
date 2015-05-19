#include "shared_controls.hpp"

#include "controls/bw_checkbox.hpp"
#include "controls/bw_combobox.hpp"
#include "controls/bw_filedialog_qi.hpp"
#include "controls/bw_textfield.hpp"
#include "controls/popup_window.hpp"
#include "helpers/qt_tree_filter.hpp"
#include "models/adapters/sequence_list_adapter.hpp"
#include "models/data_change_notifier.hpp"
#include "models/extensions/column_extension.hpp"
#include "models/extensions/component_extension.hpp"
#include "models/extensions/selection_extension.hpp"
#include "models/extensions/thumbnail_extension.hpp"
#include "models/extensions/tree_extension.hpp"
#include "models/extensions/value_extension.hpp"
#include "models/qt_list_model.hpp"
#include "models/qt_tree_model.hpp"
#include <QtQuick>

#include "reflection/i_definition_manager.hpp"
#include "reflection/reflection_macros.hpp"

#include "command_system/command_system_provider.hpp"

//==============================================================================
/*static */void SharedControls::init()
{
	qmlRegisterType< BWComboBox, 1 >("BWControls", 1, 0, "BWComboBox" );
	qmlRegisterType< PopupWindow, 1 >("BWControls", 1, 0, "PopupWindow" );
	qmlRegisterType< BWTextField, 1 >("BWControls", 1, 0, "BWTextField" );
	qmlRegisterType< BWCheckBox, 1 >("BWControls", 1, 0, "BWCheckBox" );
	qmlRegisterType< SequenceListAdapter, 1 >("BWControls", 1, 0, "SequenceList" );
	qmlRegisterType< BWFileDialogQI, 1 >( "BWControls", 1, 0, "BWFileDialogQI" );
	qmlRegisterType< QtListModel, 1 >( "BWControls", 1, 0, "BWListModel" );
	qmlRegisterType< DataChangeNotifier, 1 >(
		"BWControls", 1, 0, "BWDataChangeNotifier" );
	qmlRegisterType< QtTreeModel, 1 >( "BWControls", 1, 0, "BWTreeModel" );
	qmlRegisterType< ColumnExtension, 1 >( "BWControls", 1, 0, "ColumnExtension" );
	qmlRegisterType< ComponentExtension, 1 >( "BWControls", 1, 0, "ComponentExtension" );
	qmlRegisterType< SelectionExtension, 1 >( "BWControls", 1, 0, "SelectionExtension" );
	qmlRegisterType< ThumbnailExtension, 1 >( "BWControls", 1, 0, "ThumbnailExtension" );
	qmlRegisterType< TreeExtension, 1 >( "BWControls", 1, 0, "TreeExtension" );
	qmlRegisterType< ValueExtension, 1 >( "BWControls", 1, 0, "ValueExtension" );
	qmlRegisterType< QtTreeFilter, 1 >( "BWControls", 1, 0, "BWTreeFilter" );
}


//==============================================================================
/*static */void SharedControls::initDefs(
	IDefinitionManager & definitionManager )
{
}

//==============================================================================
/*static */void SharedControls::initCommands(
	CommandSystemProvider & commandSystemProvider )
{
}

