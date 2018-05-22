import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Item {
    id: initBox
    property alias labelText: text1.text
    function openPopup(){
        rect1.visible=true
    }
    function closePopup(){
       rect1.visible=false
    }
    anchors.fill: parent
    signal boxClicked()
    FuzzyPanel {
        id: rect1
        //dim:false
        target: backGImage
        visible: false
        //closePolicy: Popup.NoAutoClose
        width: parent.width
        height: parent.height
        //modal: false
        //Material.elevation:0

        MouseArea{
            anchors.fill: parent
            onClicked: {
                boxClicked()
            }
        }

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
            text:""
            anchors.bottom: control.top
            wrapMode: Text.WordWrap
            anchors.bottomMargin: 15
            clip: false
            width:control.width
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
        }
    }

    Component.onCompleted:{
    }
}
