import QtQuick 2.3
import QtQuick.Layouts 1.1
import BWControls 1.0

//TODO:
// Would be nice if this just inherited vector4_component and turned off the last value
// but doing so caused a major crash.
// Need to add in proper min/max values and required decimal points.

WGExpandingRowLayout {
    anchors.fill: parent
    height: parent.height

    WGExpandingRowLayout{
        Layout.fillWidth: true
        Layout.minimumWidth: 60
        Layout.preferredHeight: parent.height
        WGLabel{
            Layout.preferredWidth: 16
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            text: "X: "
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        WGNumberBox {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            number: itemData.Value.x
            maximumValue: 2147483647
            minimumValue: -2147483647

            onNumberChanged: {
                itemData.Value.x = number
            }
        }
    }

    WGExpandingRowLayout { //Spacer between each vector control
        Layout.fillWidth: true
        Layout.maximumWidth: 10
        Layout.preferredWidth: 5
        Layout.preferredHeight: parent.height
        Rectangle {
            Layout.fillWidth: true
            color: "transparent"
            Layout.preferredHeight: parent.height
        }
    }

    WGExpandingRowLayout{
        Layout.fillWidth: true
        Layout.minimumWidth: 60
        Layout.preferredHeight: parent.height
        WGLabel{
            Layout.preferredWidth: 16
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            text: "Y: "
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        WGNumberBox {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            number: itemData.Value.y
            maximumValue: 2147483647
            minimumValue: -2147483647

            onNumberChanged: {
                itemData.Value.y = number
            }
        }
    }

    WGExpandingRowLayout { //Spacer between each vector control
        Layout.fillWidth: true
        Layout.maximumWidth: 10
        Layout.preferredWidth: 5
        Layout.preferredHeight: parent.height
        Rectangle {
            Layout.fillWidth: true
            color: "transparent"
            Layout.preferredHeight: parent.height
        }
    }

    WGExpandingRowLayout{
        Layout.fillWidth: true
        Layout.minimumWidth: 60
        Layout.preferredHeight: parent.height
        WGLabel{
            Layout.preferredWidth: 16
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            text: "Z: "
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        WGNumberBox {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            number: itemData.Value.z
            maximumValue: 2147483647
            minimumValue: -2147483647

            onNumberChanged: {
                itemData.Value.z = number
            }
        }
    }
}
