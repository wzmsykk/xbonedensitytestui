import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.settings 1.0
import "../components"
Grid {
    id: ts
    anchors.fill: parent
    columns: 1
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
        title: qsTr("Scan Engine Only")
        focus: true
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            popupLoader.push("../pages/AutoScanEng.qml","show2")
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
        }

    }
    FlatButton {
        id: ps02
        title: qsTr("Regular Auto Scan")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            popupLoader.push("../pages/RegularAutoScan.qml","show2")
            ps02c.enabled=true
        }
        Connections{
            id:ps02c
            target:popupLoader.item
            enabled:false
            ignoreUnknownSignals: true
            onCanceled:{
                popupLoader.pop()
            }
        }
    }
    FlatButton {
        id: ps03
        title: qsTr("Final Scan with Report")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onButtonClicked: {
            popupLoader.push("../pages/FinalScanRep.qml","show2")
            ps03c.enabled=true
        }
        Connections{
            id:ps03c
            target:popupLoader.item
            enabled:false
            ignoreUnknownSignals: true
            onCanceled:{
                popupLoader.pop()
            }
        }

    }
    FlatButton {
        id: ps04
        title: qsTr("Scan Info")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3

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
