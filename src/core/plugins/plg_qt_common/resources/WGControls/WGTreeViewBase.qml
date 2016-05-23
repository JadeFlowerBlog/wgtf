import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQml.Models 2.2
import WGControls 2.0

ListView {
    id: treeViewBase

    property var view
    property real depth: 0

    signal itemPressed(var mouse, var itemIndex, var rowIndex)
    signal itemClicked(var mouse, var itemIndex, var rowIndex)
    signal itemDoubleClicked(var mouse, var itemIndex, var rowIndex)

    property var __onItemPressed: function(mouse, itemIndex, rowIndex) {}
    property var __onItemClicked: function(mouse, itemIndex, rowIndex) {}
    property var __onItemDoubleClicked: function(mouse, itemIndex, rowIndex) {}

    onItemPressed: __onItemPressed(mouse, itemIndex, rowIndex)
    onItemClicked: __onItemClicked(mouse, itemIndex, rowIndex)
    onItemDoubleClicked: __onItemDoubleClicked(mouse, itemIndex, rowIndex)

    delegate: Item {
        height: childrenRect.height
        width: childrenRect.width

        WGItemRow {
            id: itemRow
            columnDelegates: view.columnDelegates
            columnSequence: view.columnSequence
            columnWidths: view.columnWidths
            columnSpacing: view.columnSpacing
            isSelected: view.selectionModel.isSelected(modelIndex)

            Connections {
                target: view.selectionModel
                onSelectionChanged: {
                    itemRow.isSelected = view.selectionModel.isSelected(modelIndex)
                }
            }

            onItemPressed: treeViewBase.itemPressed(mouse, itemIndex, modelIndex)
            onItemClicked: treeViewBase.itemClicked(mouse, itemIndex, modelIndex)
            onItemDoubleClicked: treeViewBase.itemDoubleClicked(mouse, itemIndex, modelIndex)
        }

        Item {
            id: childItems
            anchors.top: itemRow.bottom

            Loader {
                active: hasChildren && expanded

                Component.onCompleted: {
                    setSource("WGTreeViewBase.qml", {
                        "interactive": false,
                        "width": Qt.binding( function() { return treeViewBase.width - childItems.x } ),
                        "height": Qt.binding( function() { return treeViewBase.height - childItems.y + treeViewBase.contentY } ),
                        
                        "view": Qt.binding( function() { return treeViewBase.view } ),
                        "model": Qt.binding( function() { return childModel } ),
                        "depth": Qt.binding( function() { return treeViewBase.depth + 1 } ),

                        "__onItemPressed": function(mouse, itemIndex, rowIndex) { treeViewBase.itemPressed(mouse, itemIndex, rowIndex) },
                        "__onItemClicked": function(mouse, itemIndex, rowIndex) { treeViewBase.itemClicked(mouse, itemIndex, rowIndex) },
                        "__onItemDoubleClicked": function(mouse, itemIndex, rowIndex) { treeViewBase.itemDoubleClicked(mouse, itemIndex, rowIndex) }
                    })

                    childItems.width = Qt.binding( function() { return active ? item.contentWidth : 0 } )
                    childItems.height = Qt.binding( function() { return active ? item.contentHeight : 0 } )
                }
            }
        }
    }
}
