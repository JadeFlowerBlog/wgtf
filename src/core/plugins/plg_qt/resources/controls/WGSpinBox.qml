/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


//Spinbox that reimplements the standard QML Spinbox to fix mouseWheel problems and have optional arrow buttons.
//Features:
//Single leftclick drag up and down over any part of the control will increase/decrease values
//Toggling pressed Ctrl key will accellerate the value change on drag
//leftclicked up and down buttons will increment stepSize the value
//Rightclicked buttons will set the control to zero or minimum value if > 0
//Accepts Keypad numerical input
//Mouse Wheel up/down increases/decreases values

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.2

/*!
    \qmltype SpinBox
    \inqmlmodule QtQuick.Controls
    \since 5.1
    \ingroup controls
    \brief Provides a spin box control.

    SpinBox allows the user to choose a value by clicking the up or down buttons, or by
    pressing up or down on the keyboard. The user can also type the value in manually.

    By default the SpinBox provides discrete values in the range [0-99] with a \l stepSize of 1 and 0 \l decimals.

    \code
    SpinBox {
        id: spinbox
    }
    \endcode

    Note that if you require decimal values you will need to set the \l decimals to a non 0 value.

    \code
    SpinBox {
        id: spinbox
        decimals: 2
    }
    \endcode

*/

Control {
    id: spinbox

    // Todo This should probably be in panelprops?
    property var spinBoxSpinnerSize: 16

    /*!
        \qmlproperty real SpinBox::value

        The value of this SpinBox, clamped to \l minimumValue and \l maximumValue.

        The default value is \c{0.0}.
    */
    property alias value: validator.value

    /*!
        \qmlproperty real SpinBox::minimumValue

        The minimum value of the SpinBox range.
        The \l value is clamped to this value.

        The default value is \c{0.0}.
    */
    property alias minimumValue: validator.minimumValue

    /*!
        \qmlproperty real SpinBox::maximumValue

        The maximum value of the SpinBox range.
        The \l value is clamped to this value. If maximumValue is smaller than
        \l minimumValue, \l minimumValue will be enforced.

        The default value is \c{99}.
    */
    property alias maximumValue: validator.maximumValue

    /*! \qmlproperty real SpinBox::stepSize
        The amount by which the \l value is incremented/decremented when a
        spin button is pressed.

        The default value is \c{1.0}.
    */
    property alias stepSize: validator.stepSize

    /*! \qmlproperty string SpinBox::suffix
        The suffix for the value. I.e "cm" */
    property alias suffix: validator.suffix

    /*! \qmlproperty string SpinBox::prefix
        The prefix for the value. I.e "$" */
    property alias prefix: validator.prefix

    /*! \qmlproperty int SpinBox::decimals
      This property indicates the amount of decimals.
      Note that if you enter more decimals than specified, they will
      be truncated to the specified amount of decimal places.
      The default value is \c{0}.
    */
    property alias decimals: validator.decimals

    /*! \qmlproperty font SpinBox::font

        This property indicates the current font used by the SpinBox.
    */
    property alias font: input.font

    /*! This property indicates whether the Spinbox should get active
      focus when pressed.
      The default value is \c true.
    */
    property bool activeFocusOnPress: true

    /*! \qmlproperty enumeration horizontalAlignment
        \since QtQuick.Controls 1.1

        This property indicates how the content is horizontally aligned
        within the text field.

        The supported values are:
        \list
        \li Qt.AlignLeft
        \li Qt.AlignHCenter
        \li Qt.AlignRight
        \endlist

      The default value is style dependent.
    */
    property int horizontalAlignment: Qt.AlignLeft

    /*!
        \qmlproperty bool SpinBox::hovered

        This property indicates whether the control is being hovered.
    */
    readonly property alias hovered: mouseArea.containsMouse

    property bool dragging_: mouseArea.drag.active

    /*!
        \qmlsignal SpinBox::editingFinished()
        \since QtQuick.Controls 1.1

        This signal is emitted when the Return or Enter key is pressed or
        the control loses focus. Note that if there is a validator
        set on the control and enter/return is pressed, this signal will
        only be emitted if the validator returns an acceptable state.

        The corresponding handler is \c onEditingFinished.
    */
    signal editingFinished()

    //style: Qt.createComponent(Settings.style + "/SpinBoxStyle.qml", spinbox)


    property alias __text: input.text

    property alias readOnly: input.readOnly

    property alias noFrame_: input.noFrame_

    property string label_: ""

    property bool noArrows_: false

    property alias b_Target: dataBinding.target
    property alias b_Property: dataBinding.property
    property alias b_Value: dataBinding.value

    property alias textColor: input.textColor

    /*! \internal */

    //increments the value

    function tickValue(amount) {
        value += amount
        if (activeFocus)
            input.selectValue()
    }

    property real originalValue_: 0 //the value before dragging
    property real tempValueAdd_: 0 //the amount to add to the original value

    property bool fastDrag_: false //if ctrl held down increment is much faster
    property real fakeZero_: 0  //a fake zero after ctrl is held or released
    property bool useValidatorOnInputText: true // Let the validator update the input.text

    /*! \internal */

    Binding {
        id: dataBinding

    }

    Text {
        id: maxSizeHint
        text: prefix + maximumValue.toFixed(decimals) + suffix
        font: input.font
        visible: false
    }

    Text {
        id: minSizeHint
        text: prefix + minimumValue.toFixed(decimals) + suffix
        font: input.font
        visible: false
    }

    implicitWidth: {
        if (!noArrows_){
            maxSizeHint.paintedWidth + panelProps.doubleMargin_ + arrowBox.width
        } else {
            maxSizeHint.paintedWidth + panelProps.doubleMargin_
        }
    }

    activeFocusOnTab: true

    onActiveFocusChanged: if (activeFocus) input.selectValue()

    Accessible.name: input.text
    Accessible.role: Accessible.SpinBox


    WGTextBox {
        id: input
        clip: text.paintedWidth > width
        anchors.fill: parent

        assetBrowserContextMenu: false

        focus: true
        activeFocusOnPress: spinbox.activeFocusOnPress

        horizontalAlignment: spinbox.horizontalAlignment
        verticalAlignment: Qt.AlignVCenter
        inputMethodHints: Qt.ImhFormattedNumbersOnly

        validator: SpinBoxValidator {
            id: validator
            property bool ready: false // Delay validation until all properties are ready
            onTextChanged: 
			{
                if (ready && useValidatorOnInputText)
				{
					input.text = validator.text
				}
			}
			
            Component.onCompleted:
			{
                if (useValidatorOnInputText)
				{
					input.text = validator.text
				}
				ready = true
			}
        }
		
        onAccepted: {
            if (useValidatorOnInputText)
			{
				input.text = validator.text
			}
            selectValue()
        }

        //This breaks Tab focus... but not sure if it does anything else useful. Leaving here for now.
        //Keys.forwardTo: spinbox

        onEditingFinished: spinbox.editingFinished()

        function selectValue() {
            select(prefix.length, text.length - suffix.length)
        }
    }

    // Spinbox arrow buttons
    Rectangle {
        id: arrowBox
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - panelProps.standardBorder_
        color: "transparent"
        height: parent.height
        width: spinBoxSpinnerSize

        WGButtonFrame {
            id: arrowUpButtonFrame            
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -(parent.height / 4)

            anchors.horizontalCenter: parent.horizontalCenter

            property var originalInnerBorderColor_: palette.LighterShade
            property var originalHighlightColor_: "transparent"
            property var originalBorderColor_: palette.DarkerShade

            height: parent.height / 2
            radius: 0

            visible: !noArrows_

            width: parent.width

            //up arrow
            Text {
                id: upArrowText
                color : {
                    if (spinbox.enabled && !input.readOnly){
                        palette.NeutralTextColor
                    } else {
                        palette.DisabledTextColor
                    }
                }

                anchors.horizontalCenter: parent.horizontalCenter                
                anchors.verticalCenter: parent.verticalCenter

                font.family : "Marlett"
                font.pixelSize: 2 * Math.round(parent.height/2)

                renderType: Text.QtRendering
                text : "t"
            }
        }

        WGButtonFrame {
            id: arrowDownButtonFrame            
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: parent.height / 4

            anchors.horizontalCenter: parent.horizontalCenter

            property var originalInnerBorderColor_: palette.LighterShade
            property var originalHighlightColor_: "transparent"
            property var originalBorderColor_: palette.DarkerShade

            height: parent.height / 2
            radius: 0

            visible: !noArrows_

            width: parent.width

            //down arrow
            Text {
                color : {
                    if (spinbox.enabled && !input.readOnly){
                        palette.NeutralTextColor
                    } else {
                        palette.DisabledTextColor
                    }
                }

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                font.family : "Marlett"
                font.pixelSize: 2 * Math.round(parent.height/2)
                renderType: Text.QtRendering
                text : "u"
            }
        }
    }

    //invisible line that handles incrementing the value by dragging
    Rectangle {
        id: dragBar
        height: 1
        width: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top        
        color: "transparent"

        property int modifier: fastDrag_ ? 1 : 10

        Drag.active: mouseArea.drag.active && !input.readOnly

        states: [
            State {
                when: dragBar.Drag.active

                AnchorChanges {
                    target: dragBar
                    anchors.top: undefined
                }
            }
        ]

        //add the position of the bar to the value. Use a fakezero if fastDrag has been toggled.
        onYChanged:{
            if (Drag.active){
                tempValueAdd_ = (((-y + fakeZero_) / modifier) * stepSize)
                validator.value = originalValue_ + tempValueAdd_
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        preventStealing: true
        propagateComposedEvents: true //Allow context menu for textbox

        drag.target: dragBar
        drag.axis: Drag.YAxis

        onPressed:{
            if (activeFocusOnPress) input.forceActiveFocus()
        }

        //start changing the value via dragging dragBar
        drag.onActiveChanged: {
            if (mouseArea.drag.active) {
                originalValue_ = validator.value
            } else {
                tempValueAdd_ = 0
                originalValue_ = 0
                fakeZero_ = 0
            }
        }

        onWheel: {
            if (!input.readOnly && input.activeFocus){
                if (wheel.angleDelta.y > 0){
                    tickValue(1)
                } else {
                    tickValue(-1)
                }

                editingFinished()
            }
        }

        //add/subtract by one if an arrow is clicked. Set to minimum if arrows are right clicked
        onClicked: {            
            if (!noArrows_ && !input.readOnly){
                var arrowPoint = mouseArea.mapToItem(arrowBox, mouse.x, mouse.y)

                if (arrowBox.contains(Qt.point(arrowPoint.x, arrowPoint.y))){
                    if(mouse.button == Qt.RightButton){  
                        validator.value = minimumValue
                    } else if (arrowPoint.y < arrowBox.height / 2){
                        tickValue(1)
                        //On released would not register for upButtonMouseArea, so colour is changed here
                        arrowUpButtonFrame.innerBorderColor_ = arrowUpButtonFrame.originalInnerBorderColor_
                        arrowUpButtonFrame.highlightColor_ = arrowUpButtonFrame.originalHighlightColor_
                    } else if (arrowPoint.y > arrowBox.height / 2){
                        tickValue(-1)
                        arrowDownButtonFrame.innerBorderColor_ = arrowDownButtonFrame.originalInnerBorderColor_
                        arrowDownButtonFrame.highlightColor_ = arrowDownButtonFrame.originalHighlightColor_
                    }
                    editingFinished()
                }
                else if (mouse.button == Qt.RightButton){ //mouse is over text box
                    mouse.accepted = false //pass right click to textbox for context menu
                }
            }
            //need if menu for readonly.. you can copy with readonly but not paste or cut!
        }

        onReleased: {            
            arrowUpButtonFrame.innerBorderColor_ = arrowUpButtonFrame.originalInnerBorderColor_
            arrowUpButtonFrame.highlightColor_ = arrowUpButtonFrame.originalHighlightColor_
            arrowDownButtonFrame.innerBorderColor_ = arrowDownButtonFrame.originalInnerBorderColor_
            arrowDownButtonFrame.highlightColor_ = arrowDownButtonFrame.originalHighlightColor_
        }

        //Mouse area for triggering click colour change in WGFrameButtons - arrowUpButtonFrame
        MouseArea {
            id: upButtonMouseArea
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -(parent.height / 4)
            height: parent.height / 2
            width: spinBoxSpinnerSize
            propagateComposedEvents: true
            hoverEnabled: true

            onClicked: {                
                mouse.accepted = false
            }

            onEntered: {                
                arrowUpButtonFrame.highlightColor_ = palette.LighterShade
            }

            onExited: {                
                arrowUpButtonFrame.highlightColor_ = arrowUpButtonFrame.originalHighlightColor_
            }

            onPressed: {                
                arrowUpButtonFrame.innerBorderColor_ = palette.DarkerShade
                arrowUpButtonFrame.highlightColor_ = palette.DarkerShade
                mouse.accepted = false
            }

            onReleased: {                
                arrowUpButtonFrame.innerBorderColor_ = arrowUpButtonFrame.originalInnerBorderColor_
                arrowUpButtonFrame.highlightColor_ = arrowUpButtonFrame.originalHighlightColor_
            }
        }

        //Mouse area for triggering click colour change in WGFrameButtons - arrowDownButtonFrame
        MouseArea {
            id: downButtonMouseArea
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: parent.height / 4
            height: parent.height / 2
            width: spinBoxSpinnerSize
            propagateComposedEvents: true
            hoverEnabled: true

            onClicked: {                
                mouse.accepted = false
            }

            onEntered: {                
                arrowDownButtonFrame.highlightColor_ = palette.LighterShade
            }

            onExited: {                
                arrowDownButtonFrame.highlightColor_ = arrowDownButtonFrame.originalHighlightColor_
            }

            onPressed: {                
                arrowDownButtonFrame.innerBorderColor_ = palette.DarkerShade
                arrowDownButtonFrame.highlightColor_ = palette.DarkerShade
                mouse.accepted = false
            }

            onReleased: {                
                arrowDownButtonFrame.innerBorderColor_ = arrowDownButtonFrame.originalInnerBorderColor_
                arrowDownButtonFrame.highlightColor_ = arrowDownButtonFrame.originalHighlightColor_
            }
        }
    }

    Keys.onUpPressed: {
        if (!input.readOnly){
            tickValue(1)
        }
    }
    Keys.onDownPressed: {
        if (!input.readOnly){
            tickValue(-1)
        }
    }

    //toggle fastDrag_ with Ctrl. Also set a new zero point so current value can be changed instead of the original value.

    Keys.onPressed: {
        if (event.key == Qt.Key_Control){
            fastDrag_ = true
            if (dragBar.Drag.active){
                validator.value = originalValue_ + tempValueAdd_
                originalValue_ = validator.value
                tempValueAdd_ = 0
                fakeZero_ = dragBar.y
            }
        }
    }
    Keys.onReleased: {        
        if (event.key == Qt.Key_Control){
            fastDrag_ = false
            if (dragBar.Drag.active){
                validator.value = originalValue_ + tempValueAdd_
                originalValue_ = validator.value
                tempValueAdd_ = 0
                fakeZero_ = dragBar.y
            }
        }
    }
}
