import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.settings 1.0
import "../components"
FuzzyPanel {
    anchors.fill: parent
    implicitHeight: 480
    implicitWidth: 600
    signal accepted
    signal canceled
    target: backGImage
Grid {
    id: ts
    anchors.fill: parent
    columns: 2
    rows: 3
    spacing: 6
    padding: 6
    signal accepted
    signal canceled
    transitions: Transition {
       AnchorAnimation{
           duration: 20000
       }
    }


    FlatButton {
        id: ps01
        title: qsTr("LCD Display")
        focus: true
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            popupLoader.push("../pages/LCDTest.qml","show")
            ps01c.target=popupLoader.currItem()
            ps01c.enabled=true
        }
        Connections{
            id:ps01c
            target:popupLoader.item
            enabled:false
            ignoreUnknownSignals: true
            onCanceled:{
                popupLoader.pop()
            }
            onAccepted:{
                popupLoader.pop()
            }
        }

    }
    FlatButton {
        id: ps02
        title: qsTr("Backlight On")
        property bool light: true
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            if(light===true){
            ps02.title= qsTr("Backlight Off")
                light=false
            }else{
                ps02.title= qsTr("Backlight On")
                    light=true
            }
        }

    }
    FlatButton {
        id: ps03
        title: qsTr("KeyBoard test")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            popupLoader.push("../pages/KeyBoardTest.qml","show")
            ps03c.target=popupLoader.currItem()
            ps03c.enabled=true
        }
        Connections{
            id:ps03c
            target:popupLoader.item
            enabled:false
            ignoreUnknownSignals: true
            onCanceled:{
                console.log("ps03cpop")
                popupLoader.pop()
            }
            onAccepted:{
                popupLoader.pop()
            }
        }

    }
    FlatButton {
        id: ps04
        title: qsTr("X-Ray LED on")
        //model: ["colored","grey"]
        property bool light: true
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            if(light===true){
            ps04.title= qsTr("X-Ray LED on")
                light=false
            }else{
                ps04.title= qsTr("X-Ray LED off")
                    light=true
            }
        }
    }


}
Component.onCompleted: {

    ps01.forceFocus()
    acceptButton.pushRole([accept,qsTr("Ok"),"idle"])
    cancelButton.pushRole([cancel,qsTr("Cancel"),"idle"])


}
Component.onDestruction: {

    acceptButton.popRole()
    cancelButton.popRole()
}

Connections {
    id: accept
    target: acceptButton
    enabled: true
    onButtonClicked: {

        accepted()
    }
}
Connections {
    id: cancel
    target: cancelButton
    enabled: true
    onButtonClicked: {

       canceled()
    }
}
}
