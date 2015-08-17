import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

ListView {
    id: listView
	boundsBehavior: Flickable.StopAtBounds
    currentIndex: -1
	clip: true
	leftMargin: 2
	rightMargin: 2
	topMargin: 2
	bottomMargin: 2
	spacing: 0
	
	signal rowClicked(var mouse, var modelIndex)
	signal rowDoubleClicked(var mouse, var modelIndex)

	property var selectionExtension: null
	property var columnDelegates: []
	property real minimumRowHeight: defaultSpacing.minimumRowHeight
	property real columnSpacing: 1
	property real selectionMargin: 0
	property bool enableVerticalScrollBar: true

	property QtObject parentView: listView

	property var defaultColumnDelegate: Component {
		Item {
			Layout.fillWidth: true
			Layout.preferredHeight: minimumRowHeight
			
			Text {
				id: value
				clip: true
				anchors.left: parent.left
				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.margins: 4
				verticalAlignment: Text.AlignVCenter
				text: typeof itemData.Value === "string" ? itemData.Value : typeof itemData.Value
				color: palette.TextColor
			}
		}
	}

	delegate: WGListViewRowDelegate {
		anchors.left: parent.left
		width: parent.width - leftMargin - rightMargin - (enableVerticalScrollBar ? verticalScrollBar.collapsedWidth : 0) - 1
		defaultColumnDelegate: listView.defaultColumnDelegate
		columnDelegates: listView.columnDelegates
		selectionExtension: listView.selectionExtension
		
		onClicked: {
			var modelIndex = listView.model.index(rowIndex, 0);
			listView.rowClicked(mouse, modelIndex);
		}
		
		onDoubleClicked: {
			var modelIndex = listView.model.index(rowIndex, 0);
			listView.rowDoubleClicked(mouse, modelIndex);
		}
	}

	WGScrollBar {
		id: verticalScrollBar
		width: defaultSpacing.rightMargin
		anchors.top: listView.top
		anchors.right: listView.right
		anchors.bottom: listView.bottom
		orientation: Qt.Vertical
		position: listView.visibleArea.yPosition
		pageSize: listView.visibleArea.heightRatio
		scrollFlickable: listView
		visible: listView.contentHeight > listView.height && enableVerticalScrollBar
	}
}
