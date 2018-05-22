import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle {
    id: tile01
    property alias text:mylabel.text
    signal tileClicked()
    width: tileWidth
    height: tileHeight
    implicitHeight: 80
    implicitWidth: 80
    opacity: 0.75
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    MouseArea {
        anchors.fill: parent
        onClicked: {
            tileClicked()
        }
        onPressed: {
            tile01.color="grey"
        }
        onReleased: {
            tile01.color="white"
        }
    }
    Label{
        id:mylabel
        text:""
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "black"
        font.pixelSize: 40
        anchors.fill: parent
        anchors.centerIn: parent

    }
}
