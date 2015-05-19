import QtQuick 2.0
import QtQuick.Controls 1.1

//WIP

Item {
    property bool isLeft_ : true
    property alias placeholderText : textBox.placeholderText
    property alias label_ : textBox.label_
    property alias text : textBox.text

    implicitHeight: {
        if (panelProps.rowHeight_){
            panelProps.rowHeight_
        } else {
            22
        }
    }

    TextField {
        id: textBox
        property bool noFrame_ : false
        property string label_: ""

        anchors.left: isLeft_ ? icon.right : parent.left
        anchors.right: isLeft_ ? parent.right : icon.left
        anchors.leftMargin: isLeft_ ? panelProps.rowSpacing_ : undefined
        anchors.rightMargin: isLeft_ ? undefined : panelProps.rowSpacing_

        activeFocusOnTab: enabled

        implicitHeight: {
            if (panelProps.rowHeight_){
                panelProps.rowHeight_
            } else {
                22
            }
        }

        //fix for input text being too high
        y: 1

        //Placeholder text in italics
        font.italic: text == "" ? true : false

        style: WGTextBoxStyle {

        }
    }

    Image {
        id: icon
        source: "qrc:///icons/search_16x16"
        anchors.left: isLeft_ ? parent.left : undefined
        anchors.right: isLeft_ ? undefined : parent.right
        anchors.verticalCenter: parent.verticalCenter

        width : 16
        height: 16
    }

}
