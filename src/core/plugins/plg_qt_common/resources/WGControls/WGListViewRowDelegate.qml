import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import BWControls 1.0

/*!
 \brief WGListViewRowDelegate is used within WGListView's delegate.
 WGListViewRowDelegate will load WGListViewColumnDelegate in its delegate or fall back to a default if none exists.
 WGListViewRowDelegate should only be used within the contexts of a ListView.
 See WGTreeItem for an example of its use.

*/

Item {
    id: rowDelegate
    objectName: "WGListViewRowDelegate"
    height: minimumRowHeight
    clip: true

    /*!
        This property defines the indentation before the first element on each row
        The default value is \c 0
    */
    property int indentation: 0

    /*!
        This property holds the index of the selected row in the list
    */
    property int rowIndex: index

    /*!
        This property contains a default column delegate.
        The default value is \c null
    */
    property var defaultColumnDelegate: null

    /*!
        This property contains the items to be delegated by the WGListViewRowDelegate's delegate.
        The default value is an empty list
    */
    property var columnDelegates: []

    /*!
        This property describes mouse selection behaviour
    */
    property var selectionExtension: null

    /*! This property passes the WGTreeView colourisation style information to the columnDelegates.
        When depthColourisation is used, the entire row is shifted using the indentation value.
        When it is not used only the first column is shifted.
        The default value is \c 0
    */
    property int depthColourisation: 0

    /*! This signal is sent on a single click
    */
    signal clicked(var mouse)

    //TODO: Improve documentation
    /*! This signal is sent on a double click
    */
    signal doubleClicked(var mouse)

    MouseArea {
        id: itemMouseArea
        parent: rowDelegate.parent
        anchors.fill: rowDelegate
        hoverEnabled: true

        onPressed: {
            if (mouse.button === Qt.LeftButton && selectionExtension != null)
            {
                var multiSelect = selectionExtension.multiSelect;

                if (mouse.modifiers & Qt.ControlModifier)
                {
                    Selected = !Selected;
                }
                else if (mouse.modifiers & Qt.ShiftModifier)
                {
                    if (multiSelect)
                    {
                        selectionExtension.prepareRangeSelect();
                        Selected = true;
                    }
                }
                else
                {
                    if (multiSelect)
                    {
                        selectionExtension.clearOnNextSelect();
                    }

                    Selected = true;
                }
            }
        }

        onClicked: {
            rowDelegate.clicked(mouse)

            // NOTE: Do not give the parent active focus here. The tree view and the list view have different ways to utilize
            //		 us, so giving parent focus will break keyboard input event handles.
        }

        onDoubleClicked: rowDelegate.doubleClicked(mouse)

        Rectangle {
            id: selectionHighlight
            color: palette.HighlightShade
            anchors.fill: itemMouseArea
            anchors.margins: selectionMargin
            visible: selectionExtension != null && Selected
        }

        Rectangle {
            id: mouseOverHighlight
            anchors.fill: itemMouseArea
            visible: itemMouseArea.containsMouse
            opacity: 0.5
            color: palette.HighlightShade
        }

        ListView {
            id: columns
            model: ColumnModel
            x: depthColourisation !==0 ? 0 : indentation  //When depthColourisation, indentation shifts the entire parent row
            width: parent.width - indentation
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            orientation: Qt.Horizontal
            interactive: false
            spacing: columnSpacing

            delegate: Loader {
                id: columnDelegate

                anchors.top: parent.top
                anchors.bottom: parent.bottom

                property var itemData: model
                property int rowIndex: rowDelegate.rowIndex
                property int columnIndex: index
                property int indentation: rowDelegate.indentation

                sourceComponent:
                    columnIndex < columnDelegates.length ? columnDelegates[columnIndex] :
                    defaultColumnDelegate

                onLoaded: {
                    var widthFunction = function()
                    {
                        if(columns.count > 1)
                        {
                            if (depthColourisation !==0) //row is offset
                            {
                                var wholeRowWidth = columns.width + indentation * depth
                                var otherColumns = wholeRowWidth * 0.75
                                var firstColumn = columns.width - otherColumns
                            }
                            else // rows are not offset, columns will be
                            {
                                var firstColumn = Math.max(0, Math.ceil(columns.width + indentation) * 0.25) - indentation;
                                var otherColumns = columns.width - firstColumn;
                            }

                            if(columnIndex == 0)
                            {
                                return firstColumn - columnSpacing;
                            }
                            else
                            {
                                return Math.ceil((otherColumns - columnSpacing) / (columns.count - 1));
                            }
                        }
                        else
                        {
                            return columns.width
                        }
                    }

                    item.width = Qt.binding(widthFunction);
                    rowDelegate.height = Math.max(height, minimumRowHeight);
                }
            }
        }
    }
}
