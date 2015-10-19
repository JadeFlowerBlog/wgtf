import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import WGControls 1.0


Rectangle {
	property var title: "Active Filters Control Test"
	property var layoutHints: { 'test': 0.1 }

	property var activeFilters_: activeFilters

    color: palette.MainWindowColor
	
	WGFilteredTreeModel {
		id: sampleDataTreeModel
		source: sampleDataToFilter

		filter: WGTokenizedStringFilter {
			id: stringFilter			
			filterText: activeFilters_.stringValue
			splitterChar: " "
		}

		ValueExtension {}
        ColumnExtension {}
        ComponentExtension {}
        TreeExtension {}
        ThumbnailExtension {}
		SelectionExtension {}
	}
	
    WGColumnLayout {
        anchors.fill: parent

        Rectangle {
            id: activeFiltersRect
            Layout.fillWidth: true
            Layout.minimumHeight: defaultSpacing.minimumRowHeight + defaultSpacing.doubleBorderSize
            Layout.preferredHeight: childrenRect.height
            color: "transparent"

            WGActiveFilters {
                id: activeFilters
                anchors {left: parent.left; top: parent.top; right: parent.right}
                height: childrenRect.height
                inlineTags: true
                dataModel: filtersModel
            }
        }

		WGSeparator {
			id: separator
		}

		Rectangle {
			anchors.top: separator.bottom
			Layout.fillWidth: true
            Layout.fillHeight: true
			color: "transparent"

            WGTreeView {
                id: sampleDataListView
                model: sampleDataTreeModel
                anchors.fill: parent
                columnDelegates : [defaultColumnDelegate]
            }
		}
	}
}
