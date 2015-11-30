import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQml.Models 2.1

import BWControls 1.0
import WGControls 1.0

WGPanel {

    title: qsTr("Custom Testing Panel")
    layoutHints: { 'test': 0.1 }

    color: palette.MainWindowColor

    focus: true

    property QtObject draggedObject: null
    property QtObject highlightedObject: null
    property bool draggingPanel: false
    property int draggedPanelDepth: -1

    function findFilteredObject(parentObject){
        for(var i=0; i<parentObject.children.length; i++)
        {
            if(typeof parentObject.children[i].filtered != "undefined")
            {
                if(parentObject.children[i].filtered)
                {
                    highlightedObject = parentObject.children[i]
                }
                else
                {
                    findFilteredObject(parentObject.children[i])
                }
            }
            else
            {
                findFilteredObject(parentObject.children[i])
            }
        }
    }

    WGMainPanel {
        id: mainPanel

        text: "Test Panel"
        subText: "Selected Object"

        panelDepth: 1
        choosePinned: false

        Keys.onPressed: {
            if (event.key == Qt.Key_Slash)
            {
                controlFilter.selectAll()
                controlFilter.forceActiveFocus()
            }
        }


        headerObject_ :
        WGExpandingRowLayout {

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
            }

            WGToolButton {
                id: pinButton
                Layout.preferredHeight: 18
                Layout.preferredWidth: 18

                iconSource: mainPanel.expanded_ == 1 ? "qrc:///icons/pinned_16x16" : "qrc:///icons/pin_16x16"

                checkable: true
                checked: mainPanel.choosePinned

                onCheckedChanged: {
                    mainPanel.choosePinned = checked
                    if(checked)
                    {
                        lockButton.checked = false
                    }
                }
            }

            WGToolButton {
                id: lockButton
                Layout.preferredHeight: 18
                Layout.preferredWidth: 18

                iconSource: checked ? "qrc:///icons/unlock_16x16" : "qrc:///icons/lock_16x16"

                checkable: true

                enabled: mainPanel.expanded_ > 0

                onCheckedChanged: {
                    mainPanel.chunkDragEnabled = checked
                    if(checked)
                    {
                        pinButton.checked = false
                    }
                }
            }

            WGToolButton {
                id: panelMenu
                Layout.preferredHeight: 18
                Layout.preferredWidth: 18

                iconSource: "qrc:///icons/menu_16x16"

                menu: WGMenu{
                    MenuItem {
                        text: "Copy Panel Data"
                        enabled: false
                    }
                    MenuItem {
                        text: "Paste Panel Data"
                        enabled: false
                    }
                }
            }

            WGToolButton {
                Layout.preferredHeight: 18
                Layout.preferredWidth: 18

                iconSource: "qrc:///icons/close_16x16"

                onClicked: {
                    mainWindow.close()
                }
            }
        }

        childObject_:

        ColumnLayout {
            id: rootFrame

            anchors.left: parent.left
            anchors.right: parent.right

            WGTextBox {
                id: searchBox
                visible: mainPanel.expanded_ > 0
                Layout.fillWidth: true

                WGToolButton {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: height
                    noInteraction_: true
                    opacity: 0.2
                    activeFocusOnTab: false

                    iconSource: "qrc:///icons/close_16x16"

                    onClicked: {
                        searchBox.text = ""
                    }
                }
                WGLabel {
                    text: "Search..."
                    font.italic: true
                    color: "#666666"
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    visible: searchBox.text == "" && !searchBox.focus
                }

                Component.onCompleted: {
                    mainPanel.controlFilter = this
                }

                Keys.onTabPressed: {
                    findFilteredObject(rootFrame)
                    if(highlightedObject != null)
                    {
                        highlightedObject.nextItemInFocusChain(true).forceActiveFocus()
                        searchBox.text = ""
                    }
                    else
                    {
                        searchBox.nextItemInFocusChain(true).forceActiveFocus()
                    }
                }
                onTextChanged: {
                    highlightedObject = null
                }
            }

            WGControlChunk {
                id: panelChunkOne
                parentPanel: mainPanel
                panelChunk: true
                tags: "sub panel 1 one"
                panelDepth: 1
                layoutRow: 0
                WGColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    WGProtoPanel {
                        text: "Sub Panel 1"
                        pinned: panelChunkOne.pinned
                        panelDepth: 2
                        childObject_:
                        WGColumnLayout {
                            id: panelOneFrame
                            anchors.left: parent.left
                            anchors.right: parent.right
                            property bool pinned: false
                            WGControlChunk {
                                tags: "text enter"
                                WGColumnLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        WGLabel {
                                            text: "Text:"
                                            horizontalAlignment: Text.AlignRight
                                            Layout.preferredWidth: 60
                                        }
                                        WGTextBox {
                                            Layout.fillWidth: true
                                        }
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        Rectangle {
                                            color: "transparent"
                                            Layout.preferredWidth: 60
                                        }
                                        WGPushButton {
                                            text: "Enter"
                                        }
                                    }
                                }
                            }

                            WGControlChunk {
                                tags: "slider"
                                WGExpandingRowLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    WGLabel {
                                        text: "Slider:"
                                        horizontalAlignment: Text.AlignRight
                                        Layout.preferredWidth: 60
                                    }
                                    WGSliderControl {
                                        Layout.fillWidth: true
                                        value: 50
                                        minimumValue: 0
                                        maximumValue: 100
                                    }
                                }
                            }

                            WGControlChunk {
                                tags: "booleans checkboxes option"
                                WGColumnLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        WGLabel {
                                            text: "Booleans:"
                                            horizontalAlignment: Text.AlignRight
                                            Layout.preferredWidth: 60
                                        }
                                        WGCheckBox {
                                            text: "Option 1"
                                            checked: true
                                        }
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        Rectangle {
                                            color: "transparent"
                                            Layout.preferredWidth: 60
                                        }
                                        WGCheckBox {
                                            text: "Option 2"
                                            checked: false
                                        }
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        Rectangle {
                                            color: "transparent"
                                            Layout.preferredWidth: 60
                                        }
                                        WGCheckBox {
                                            text: "Option 3"
                                            checked: false
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }


            WGControlChunk {
                id: panelChunkTwo
                parentPanel: mainPanel
                panelChunk: true
                tags: "sub panel 2 two"
                panelDepth: 1
                layoutRow: 1
                WGColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    WGProtoPanel {
                        text: "Sub Panel 2"
                        pinned: panelChunkTwo.pinned
                        panelDepth: 3
                        childObject_:
                        WGColumnLayout {
                            id: panelTwoFrame
                            anchors.left: parent.left
                            anchors.right: parent.right
                            property bool pinned: false
                            WGControlChunk {
                                tags: "data performancebar number value"
                                newItem: true
                                WGColumnLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        WGLabel {
                                            text: "Data:"
                                            horizontalAlignment: Text.AlignRight
                                            Layout.preferredWidth: 60
                                        }
                                        WGPerformanceBar {
                                            id: perfBar
                                            Layout.fillWidth: true
                                        }
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        Rectangle {
                                            color: "transparent"
                                            Layout.preferredWidth: 60
                                        }
                                        WGNumberBox {
                                            Layout.preferredWidth: 80
                                            value: 25
                                            minimumValue: 0
                                            maximumValue: 100

                                            b_Target: perfBar
                                            b_Property: "value_"
                                            b_Value: value
                                        }
                                    }
                                }
                            }



                            WGControlChunk {
                                tags: "radio boolean feature on off"
                                WGColumnLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    ExclusiveGroup {
                                        id: radioGroup
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        WGLabel {
                                            text: "Feature:"
                                            horizontalAlignment: Text.AlignRight
                                            Layout.preferredWidth: 60
                                        }
                                        WGRadioButton {
                                            text: "On"
                                            checked: true
                                            exclusiveGroup: radioGroup
                                        }
                                    }

                                    WGExpandingRowLayout {
                                        Layout.fillWidth: true
                                        Rectangle {
                                            color: "transparent"
                                            Layout.preferredWidth: 60
                                        }
                                        WGRadioButton {
                                            text: "Off"
                                            checked: false
                                            exclusiveGroup: radioGroup
                                        }
                                    }
                                }
                            }

                            WGControlChunk {
                                tags: "image click thumbnail load picture"
                                WGExpandingRowLayout {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    Rectangle {
                                        color: "transparent"
                                        Layout.preferredWidth: 60
                                    }
                                    WGThumbnailButton {
                                        id: openImage
                                        defaultText: "Click to Load an Image"
                                        Layout.preferredWidth: implicitWidth
                                    }
                                    Rectangle {
                                        color: "transparent"
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
