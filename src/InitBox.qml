import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0


Item {
    id: initBox
    property var labelText
    function openPopup(){
        rect1.open()
    }
    function closePopup(){
        rect1.close()
    }

    anchors.fill: parent

    Popup {
        id: rect1
        dim:true
        visible: false
        closePolicy: Popup.NoAutoClose
        width: parent.width
        height: parent.height
        modal: true
        z: 0

        ProgressBar {
            id: control
            x: 50
            y: 45
            width: parent.width*0.8
            anchors.verticalCenterOffset: 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            transformOrigin: Item.Center
            indeterminate: true
            padding: 2
        }
        Text {
            id: text1
            text: labelText
            anchors.bottom: control.top
            anchors.bottomMargin: 15
            clip: false
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
        }
    }

    Component.onCompleted:{
    }
}
