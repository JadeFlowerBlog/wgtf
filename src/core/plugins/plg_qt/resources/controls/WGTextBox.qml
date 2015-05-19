import QtQuick 2.0
import QtQuick.Controls 1.2

//Text entry field mostly intended for strings

TextField {
    id: textBox
    property bool noFrame_ : false
    property string label_: ""

    //context menu switches
    property bool assetBrowserContextMenu : true

    activeFocusOnTab: readOnly ? false : true

    implicitHeight: {
        if (panelProps.rowHeight_){
            panelProps.rowHeight_
        } else {
            22
        }
    }

    property alias b_Target: dataBinding.target
    property alias b_Property: dataBinding.property
    property alias b_Value: dataBinding.value

    Binding {
        id: dataBinding

    }

    //fix for input text being too high. There is a matching -1 in WGTextBoxStyle
    y: 1

    //Placeholder text in italics
    font.italic: text == "" ? true : false

    style: WGTextBoxStyle {

    }

    MouseArea {
        id: mouseAreaContextMenu
        acceptedButtons: Qt.RightButton

        propagateComposedEvents: true
        preventStealing: false
        anchors.fill: parent
        hoverEnabled: true

        cursorShape: Qt.IBeamCursor        
        onClicked:{
            var highlightedText = selectedText
            contextMenu.popup()
        }
    }

    // Some context menu items may be data driven.
    // I have added a visibility switch to contextMenu
    Menu {
        id: contextMenu
        title: "Edit"

        MenuItem {
            text: "Cut"
            shortcut: "Ctrl+X"
            enabled: readOnly == true ? false : true
            onTriggered: {                
                cut()
            }
        }

        MenuItem {
            text: "Copy"
            shortcut: "Ctrl+C"
            onTriggered: {                
                copy()
            }
        }

        MenuItem {
            text: "Paste"
            shortcut: "Ctrl+V"
            enabled: canPaste == true ? true : false
            onTriggered: {
                paste()
            }
        }

        MenuSeparator { }

        MenuItem {
            text: "Select All"
            shortcut: "Ctrl+A"
            onTriggered: {                
                selectAll()
            }
        }

        MenuSeparator { }

        MenuItem {
            text: "Find In AssetBrowser"
            shortcut: "Ctrl+?"
            visible: assetBrowserContextMenu == true ? true : false
            onTriggered: {
                console.log("Not yet implemented")
            }
        }
    }
}
