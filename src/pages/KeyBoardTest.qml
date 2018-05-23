import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import "../components"
FuzzyPanel {
    anchors.fill: parent
    implicitHeight: 480
    implicitWidth: 600
    signal accepted
    signal canceled
    target: backGImage
    function generate(){

    }

    Text{

        id:rt02
        anchors.fill: parent
       text:qsTr("KeyBoard Test")
        padding: 6
    }
    Connections {
        id: accept
        target: acceptButton
        enabled: false
        onButtonClicked: {

            //acceptButton.popRole()
            //cancelButton.popRole()
            //acceptAction01.enabled = false

            accepted()
        }
    }
    Connections {
        id: cancel
        target: cancelButton
        enabled: false
        onButtonClicked: {

            //acceptButton.popRole()
            //cancelButton.popRole()
            //acceptAction01.enabled = false

            canceled()
        }
    }
    Component.onCompleted: {

        //ps01.forceFocus()
        acceptButton.pushRole([accept,qsTr("Ok"),"idle"])
        cancelButton.pushRole([cancel,qsTr("Cancel"),"idle"])
        //thirdButton.pushRole([cancel,qsTr("Cancel"),"idle"])



    }
    Component.onDestruction: {

        acceptButton.popRole()
        cancelButton.popRole()
        //thirdButton.popRole()
    }
}
