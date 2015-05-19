import QtQuick 2.1
import QtQuick.Layouts 1.0
import BWControls 1.0

// TODO:
// * Labels should probably be set via C++/data somehow
// * Fix magic width numbers & alignments
// * Assign proper colours from palette
// * Vertically align label text with number text boxes.
// * Number text boxes should clip anything that gets too large.
// * Fix tab focus

RowLayout{
    id: vector4Layout
    anchors.left: parent.left
    anchors.right: parent.right
    spacing: 3
    height: parent.height

    // properties to change the field label.

    property string xCaption: "X"
    property string yCaption: "Y"
    property string zCaption: "Z"
    property string wCaption: "W"

    RowLayout {
        height: parent.height
        spacing: 1

        Text {
            id: xLabel
            height: parent.height
            Layout.minimumWidth: 16
            Layout.maximumWidth: 16
            text: xCaption + ":"
            color: palette.TextColor
            font.bold: true
        }

        Rectangle {
            id: xText
            height: parent.height
            Layout.fillWidth: true
            Layout.minimumWidth: 30
            color: palette.DarkShade
            border.width: 1
            border.color: palette.DarkerShade
            radius: 2

            BWTextField{
                id: xTextField
                anchors.fill: parent
                anchors.leftMargin: 2
                text: itemData_.Value.x
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.minimumWidth: 2
        Layout.maximumWidth: 2
    }

    RowLayout {
        height: parent.height
        spacing: 1

        Text {
            id: yLabel
            height: parent.height
            Layout.minimumWidth: 16
            Layout.maximumWidth: 16
            text: yCaption + ":"
            color: palette.TextColor
            font.bold: true
        }

        Rectangle {
            id: yText
            height: parent.height
            Layout.fillWidth: true
            Layout.minimumWidth: 30
            color: palette.DarkShade
            border.width: 1
            border.color: palette.DarkerShade
            radius: 2

            BWTextField{
                id: yTextField
                anchors.fill: parent
                anchors.leftMargin: 2
                text: itemData_.Value.y
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.minimumWidth: 2
        Layout.maximumWidth: 2
    }

    RowLayout {
        height: parent.height
        spacing: 1

        Text {
            id: zLabel
            height: parent.height
            Layout.minimumWidth: 16
            Layout.maximumWidth: 16
            text: zCaption + ":"
            color: palette.TextColor
            font.bold: true
        }

        Rectangle {
            id: zText
            height: parent.height
            Layout.fillWidth: true
            Layout.minimumWidth: 30
            color: palette.DarkShade
            border.width: 1
            border.color: palette.DarkerShade
            radius: 2

            BWTextField{
                id: zTextField
                anchors.fill: parent
                anchors.leftMargin: 2
                text: itemData_.Value.z
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.minimumWidth: 2
        Layout.maximumWidth: 2
    }

    RowLayout {
        height: parent.height
        spacing: 1

        Text {
            id: wLabel
            height: parent.height
            Layout.minimumWidth: 16
            Layout.maximumWidth: 16
            text: wCaption + ":"
            color: palette.TextColor
            font.bold: true
        }

        Rectangle {
            id: wText
            height: parent.height
            Layout.fillWidth: true
            Layout.minimumWidth: 30
            color: palette.DarkShade
            border.width: 1
            border.color: palette.DarkerShade
            radius: 2

            BWTextField{
                id: wTextField
                anchors.fill: parent
                anchors.leftMargin: 2
                text: itemData_.Value.w
            }
        }
    }

	Binding {
		target: itemData_
		property: "Value"
		value: Qt.vector4d( xTextField.text, yTextField.text, zTextField.text, wTextField.text )
	}
}
