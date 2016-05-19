import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import BWControls 1.0

/*!
 \brief Generates rows for both WGListView and WGTreeView.
*/
Item {
    id: itemRow
    objectName: "WGItemRow"

    width: childrenRect.width != 0 ? childrenRect.width : 1024
    height: childrenRect.height != 0 ? childrenRect.height : 1024

    /*! A list of components to be used for each column.
        Item 0 for column 0, item 1 for column 1 etc.
        If the number of columns surpasses the length of columnDelegates,
        then WGItemViewCommon will append more of the default columnDelegate.
        The default value is an empty list.
    */
    property var columnDelegates: []

    /*! This property holds a list of indexes to adapt from the model's columns
        to the view's columns.
        e.g. if the input model has 1 column, but columnSequence is [0,0,0]
             then the view can have 3 columns that lookup column 0 in the model.
        The default value is an empty list
    */
    property var columnSequence: []
    property var columnWidths: []
    property alias columnSpacing: row.spacing
    property bool selected: false
    
    /*! Stores which item is currently in focus by the keyboard.
        Often this will correspond to the selected item, but not always.
        E.g. pressing ctrl+up will move the current index, but not the selected index.
    */
    property bool isCurrent: false

    /*! Pass parameters from mouse events up to parent.
        \see columnMouseArea for original event.
        \param mouse the MouseEvent that triggered the signal.
        \param itemIndex comes from C++.
               Call to modelIndex() automatically looks up the
               "C++ model index" from the row and column.
               Index of items inside the WGItemRow.
     */
    signal itemPressed(var mouse, var itemIndex)
    signal itemClicked(var mouse, var itemIndex)
    signal itemDoubleClicked(var mouse, var itemIndex)

    /* MOVE INTO STYLE*/
    // Current selection and mouse hover
    Rectangle {
        id: backgroundArea
        anchors.fill: row
        color: palette.highlightShade
        opacity: selected ? 1 : 0.5
        visible: hoverArea.containsMouse || selected
    }

    MouseArea {
        id: hoverArea
        anchors.fill: backgroundArea
        hoverEnabled: true
    /**/

    // Controls column spacing.
    Row {
        id: row

        // Repeat columns horizontally.
        Repeater {
            id: rowRepeater

            model: SequenceList {
                id: rowModel
                model: columnModel
                sequence: columnSequence
            }

            delegate: Item {
                id: columnContainer
                width: columnWidths[index]
                height: childrenRect.height
                clip: true

                MouseArea {
                    id: columnMouseArea
                    width: columnWidths[index]
                    height: row.height
                    acceptedButtons: Qt.RightButton | Qt.LeftButton;

                    /*! Pass parameters from mouse events up to parent.
                        \param mouse the KeyEvent that triggered the signal.
                        \param itemIndex comes from C++.
                               Call to modelIndex() automatically looks up the
                               "C++ model index" from the row and column.
                               Index of items inside the WGItemRow.
                     */
                    onPressed: itemPressed(mouse, modelIndex)
                    onClicked: itemClicked(mouse, modelIndex)
                    onDoubleClicked: itemDoubleClicked(mouse, modelIndex)
                }

                // Line up columns horizontally.
                Row {
                    id: columnLayoutRow

                    /* MOVE INTO STYLE*/

                    // Add expanded/collapsed arrow.
                    Row {
                        id: iconArea
                        anchors.verticalCenter: parent.verticalCenter

                        width: childrenRect.width
                        height: childrenRect.height

                        visible: __isTree && index == 0

                        property bool __isTree: typeof expanded != "undefined"
                        property real __depth: __isTree ? depth : 0
                        property bool __hasChildren: __isTree ? hasChildren : false
                        property bool __expanded: __isTree ? expanded : false

                        // Reserve space.
                        Item {
                            width: iconArea.__depth * 10
                            height: 1
                        }

                        // Expanded/collapsed arrow for tree views.
                        Text {
                            objectName: typeof(model.display) != "undefined" ? "expandIcon_" + model.display : "expandIcon"
                            color: iconArea.__hasChildren ? iconArea.__expanded ? palette.textColor : palette.neutralTextColor : "transparent"
                            font.family : "Marlett"
                            text : iconArea.__expanded ? "\uF036" : "\uF034"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter

                            MouseArea {
                                anchors.fill: parent
                                enabled: iconArea.__hasChildren
                                onPressed: {
                                    expanded = !expanded;
                                }
                            }
                        }
                    }
                    /**/

                    // Actual columns added after arrow
                    Loader {
                        id: columnDelegateLoader
                        property var itemData: model
                        property int itemWidth: columnWidths[index] - x
                        sourceComponent: itemRow.columnDelegates[index]
                    }
                }
            }
        }
    }
}
