import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import WGControls 1.0

/*!
 \brief A control used to represent breadcrumb navigation of a tree view

Example:
\code{.js}
WGBreadcrumbs {
    id: breadcrumbsFrame
    dataModel: breadcrumbsModel
}
\endcode
*/

Rectangle {
    id: rootFrame

    // Public properties
    /*! This property holds the dataModel containing all breadcrumbs data */
    property var dataModel

    /*! These properties holds references to various components used to determine the size of the frame */
    property var breadcrumbRepeater_: breadcrumbRepeater
    property var breadcrumbRowLayout_: breadcrumbRowLayout

    /*! This signal is sent when a top level breadcrumb is clicked */
    signal breadcrumbClicked(var index)

    /*! This signal is sent when a subitem is clicked from the child folder menu */
    signal breadcrumbChildClicked(var index, var childIndex)

    /*! This signal is sent when the user enters a path manually and hits "enter" to confirm navigation */
    signal breadcrumbPathEntered(var path)

    // Layout properties
    Layout.fillHeight: false
    Layout.preferredHeight: defaultSpacing.minimumRowHeight
    Layout.fillWidth: true
    color: "transparent"

    property bool __showBreadcrumbs: true

    // List model needed to convert the crumbs into a QML-usable model
    WGListModel {
        id: crumbsListModel
        source: rootFrame.dataModel.crumbs

        ValueExtension {}
    }

    // Mouse area over the path text box
    MouseArea {
        anchors.fill: parent
        enabled: rootFrame.__showBreadcrumbs
        hoverEnabled: true

        cursorShape: Qt.IBeamCursor

        onClicked: {
            rootFrame.__showBreadcrumbs = false
            pathTextBox.forceActiveFocus()
        }
    }

    // Text box to store the full, raw path of the breadcrumbs. May be used to manually navigate to
    // a specific path in the tree.
    WGTextBox {
        id: pathTextBox
        anchors.fill: parent
        visible: !rootFrame.__showBreadcrumbs

        //TODO MUCH LATER: Auto complete.

        text: rootFrame.dataModel.path

        onEditingFinished: {
            rootFrame.__showBreadcrumbs = true
        }

        onAccepted: {
            breadcrumbPathEntered(text)
        }
    }

    // Main layout of the breadcrumbs control.
    RowLayout {
        id: breadcrumbLayout
        anchors.fill: parent
        spacing: 0

        visible: rootFrame.__showBreadcrumbs

        Component {
            id: breadcrumbDelegate

            RowLayout {
                id: breadcrumbRowLayout
                Layout.fillWidth: false
                spacing: 1
                property var breadcrumbIndex_

                WGListModel {
                    id: subItemsListModel
                    source: Value.subItems
                    ValueExtension {}
                }

                WGLabel {
                    id: breadcrumbLabel

                    Layout.fillWidth: true
                    Layout.preferredHeight: defaultSpacing.minimumRowHeight

                    elide: Text.ElideRight

                    text: Value.displayValue

                    font.bold: true
                    font.pointSize: 11

                    color: breadcrumbMouseArea.containsMouse ? palette.TextColor : palette.NeutralTextColor;

                    Component.onCompleted: {
                        breadcrumbRowLayout.breadcrumbIndex_ = index;
                    }

                    MouseArea {
                        id: breadcrumbMouseArea
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onPressed: {
                            breadcrumbClicked( index );
                        }
                    }
                }

                WGToolButton {
                    visible: index < breadcrumbRepeater.count - 1

                    Layout.preferredWidth: 16
                    Layout.preferredHeight: defaultSpacing.minimumRowHeight
                    showMenuIndicator: false

                    iconSource: "icons/arrow_right_small_16x16.png"

                    menu: WGMenu {
                        id: siblingFolderMenu

                        Instantiator {
                            model: subItemsListModel

                            delegate: MenuItem {
                                text: Value.displayValue
                                onTriggered: {
                                    breadcrumbChildClicked(breadcrumbRowLayout.breadcrumbIndex_, index);
                                }
                            }

                            onObjectAdded: siblingFolderMenu.insertItem(index, object)
                            onObjectRemoved: siblingFolderMenu.removeItem(object)
                        }
                    }
                }
            }
        }

        WGExpandingRowLayout {
            id: breadcrumbRowLayout
            Layout.fillWidth: true
            Layout.preferredHeight: defaultSpacing.minimumRowHeight + defaultSpacing.doubleBorderSize

            onWidthChanged: checkAssetBrowserWidth()

            spacing: 1

            Repeater {
                id: breadcrumbRepeater
                model: crumbsListModel
                delegate: breadcrumbDelegate

                onItemAdded: {
                    pathTextBox.text = rootFrame.dataModel.path;
                }

                onItemRemoved: {
                    pathTextBox.text = rootFrame.dataModel.path;
                }
            }
        }
    }
} // rootFrame
