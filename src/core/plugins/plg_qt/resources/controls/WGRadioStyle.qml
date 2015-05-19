import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.2

RadioButtonStyle {
    label: Text {
        text: control.text
        color: control.enabled ? palette.TextColor : palette.DisabledTextColor

        renderType: Text.NativeRendering

        //coloured border around the label when the control has focus
        Rectangle {
            height: parent.height + panelProps.doubleBorder_
            width: parent.width + panelProps.standardMargin_
            anchors.centerIn: parent
            visible: control.activeFocus
            color: "transparent"
            radius: panelProps.halfRadius_
            border.width: panelProps.standardBorder_
            border.color: palette.HighlightShade
        }
    }
    indicator: WGTextBoxFrame {
        id: radioFrame
        implicitWidth: 13
        implicitHeight: 13
        radius: 7

        color: {
            if (!control.noFrame_ && control.enabled){
                palette.TextBoxColor
            } else {
                "transparent"
            }
        }

        border.color: {
            if (control.enabled && !control.noFrame) {
                palette.DarkestShade
            } else if (control.enabled && control.noFrame){
                "transparent"
            } else if (!control.enabled){
                palette.DarkerShade
            }
        }

        Rectangle {
            visible: control.checked
            color: control.enabled ? palette.HighlightColor : palette.LightestShade
            radius: 7
            anchors.fill: parent
            anchors.margins:2
            border.width: panelProps.standardBorder_
            border.color: palette.DarkerShade
        }
    }
}
