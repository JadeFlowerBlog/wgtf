import QtQuick 2.3
import QtQuick.Controls 1.2
import WGControls 1.0

 
WGContextArea
{
    id: contextArea

    contextMenu: WGMenu
    {
        MenuItem
        {
            text: qsTr("Create Node")
            onTriggered: {
                createNode(contextArea.popupPoint.x, contextArea.popupPoint.y, "Test Node From QML");
            }
        }
    }
}
