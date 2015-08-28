import QtQuick 2.3
import QtQuick.Controls 1.2

/*!
 \brief Scrolling Panel. Recommend childObject_ be a WGColumnLayout or similar
 Use this class for a scroll panel intended to fill the area, eg. the ApplicationWindow
 Use WGSubScrollPanel if the parent depends on this height eg in a WGSubPanel
 Use WGColumnScrollPanel if you cannot anchor to top or bottom eg in a WGColumnLayout

\code{.js}
WGScrollPanel{
    childObject_:
        WGColumnLayout {
            WGInternalPanel{
                text: "SubPanel"
                clipContents_: true
                expanded_: true
                childObject_ :
                    WGColumnLayout{
                        WGTextBox {
                            width: 150
                            placeholderText: "Text Field"
                        }
                    }
            }
        }
}
\endcode
*/

Item {
    objectName: "WGScrollPanel"
    id: scrollableFrame

    property Component childObject_

    //TODO: This breaks the UI. Either fix or hide from user
    /*! \internal */
    property bool vertical: true

    //not anchors.fill because of inherited classes reassigning top & bottom
    anchors {left: parent.left; right: parent.right; top: parent.top; bottom: parent.bottom}

    Flickable {
        id: scrollPanel

        anchors.fill: parent

        anchors.leftMargin: defaultSpacing.leftMargin
        anchors.rightMargin: defaultSpacing.rightMargin
        anchors.topMargin: defaultSpacing.topBottomMargin
        anchors.bottomMargin: defaultSpacing.topBottomMargin

        flickableDirection: scrollableFrame.vertical ? Flickable.VerticalFlick : Flickable.HorizontalFlick
        boundsBehavior: Flickable.StopAtBounds

        contentHeight: scrollContent.height
        contentWidth: scrollContent.width

        Loader {
            id: scrollContent

            width: scrollableFrame.vertical ? scrollableFrame.width - defaultSpacing.leftMargin - defaultSpacing.rightMargin : undefined
            height: !scrollableFrame.vertical ? scrollableFrame.height : undefined

            sourceComponent: childObject_
        }
    }

    WGScrollBar {
         id: verticalScrollBar
         width: scrollableFrame.vertical ? defaultSpacing.rightMargin : scrollPanel.width
         height: !scrollableFrame.vertical ? defaultSpacing.topBottomMargin : scrollPanel.height
         anchors.left: scrollableFrame.vertical ? scrollPanel.right : undefined
         anchors.bottom: !scrollableFrame.vertical ? scrollPanel.bottom: undefined
         orientation: scrollableFrame.vertical ? Qt.Vertical : Qt.Horizontal
         position: scrollableFrame.vertical ? scrollPanel.visibleArea.yPosition : scrollPanel.visibleArea.xPosition
         pageSize: scrollableFrame.vertical ? scrollPanel.visibleArea.heightRatio : scrollPanel.visibleArea.widthRatio

         scrollFlickable: scrollPanel

         visible: scrollableFrame.vertical ? scrollContent.height > scrollPanel.height : scrollContent.width > scrollPanel.width
     }
}
