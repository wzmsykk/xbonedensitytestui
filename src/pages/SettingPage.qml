import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import "../components"
Grid{


    id:rt02
    anchors.fill: parent
    columns: 2
    rows:3
    spacing: 6
    padding: 6
    signal accepted()
    function recoverState(){
        acceptAction01.enabled=true
        rt02.visible=true
        rt02.enabled=true
        cancelButton.state="invisible"
        popupLoader.pop()
        popupLoader.state="hide"
        inputPanel.state="hide"
    }
    function holdState(){
         acceptAction01.enabled=false

        rt02.visible=false
        rt02.enabled=false
    }

    FlatButton {
        width: (parent.width-3*rt02.spacing)/rt02.columns
        height: (parent.height-5*rt02.spacing)/rt02.rows
        title: qsTr("Print Settings")
        onButtonClicked: {
            popupLoader.push("../pages/PrintSettings.qml")
            popupLoader.state="show"

            printSetConn.enabled=true
            holdState()
        }
        Connections{
            id:printSetConn
            target: popupLoader.item
            ignoreUnknownSignals: true
            enabled:false
            onAccepted:{
                recoverState()
                console.log("printAccepted")
                printSetConn.enabled=false
            }
            onCanceled:{
                recoverState()
                printSetConn.enabled=false
            }
        }
    }
    FlatButton {
        width: (parent.width-3*rt02.spacing)/rt02.columns
        height: (parent.height-5*rt02.spacing)/rt02.rows
        title: qsTr("Time Settings")
        onButtonClicked: {
             popupLoader.push("../pages/TimeSettings.qml")
            popupLoader.state="show2"
             inputPanel.state="show"
            timeSetConn.enabled=true
            holdState()
        }
        Connections{
            id:timeSetConn
            target: popupLoader.item
            ignoreUnknownSignals: true
            enabled:false
            onAccepted:{
                recoverState()
                 timeSetConn.enabled=false
            }
            onCanceled:{
                recoverState()
                timeSetConn.enabled=false
            }
        }
    }
    FlatButton{
        width: (parent.width-3*rt02.spacing)/rt02.columns
        height: (parent.height-5*rt02.spacing)/rt02.rows
        title: qsTr("Default Info Settings")
        onButtonClicked: {
            currInfo=defaultInfoSet
            popupLoader.push("../pages/PatientSet.qml")
            popupLoader.state="show2"
             inputPanel.state="show"
             discom.enabled=true
            cancelButton.state="idle"
            holdState()
        }
        Connections{
            id:discom
            enabled:false
            target: popupLoader.item
            ignoreUnknownSignals: true
            onAccepted:{
                recoverState()

                    defaultInfoSet=currInfo
               //console.log(infoSet)
                discom.enabled=false
            }
            onCanceled:{
                recoverState()
                 discom.enabled=false
            }
        }
    }
    FlatButton{
        width: (parent.width-3*rt02.spacing)/rt02.columns
        height: (parent.height-5*rt02.spacing)/rt02.rows
        title: qsTr("Password Settings")
        onButtonClicked: {
            popupLoader.push("../pages/PasswordSettings.qml")
            popupLoader.state="show2"
            inputPanel.state="show"
             pascom.enabled=true
            cancelButton.state="idle"
            holdState()
        }
        Connections{
            id:pascom
            enabled:false
            target: popupLoader.item
            ignoreUnknownSignals: true
            onAccepted:{
                recoverState()

                    console.log("pasxcom")
                pascom.enabled=false
            }
            onCanceled:{
                recoverState()
                 pascom.enabled=false
            }
        }
    }
    FlatButton{
        width: (parent.width-3*rt02.spacing)/rt02.columns
        height: (parent.height-5*rt02.spacing)/rt02.rows
        title: qsTr("Other Settings")
        onButtonClicked: {
            popupLoader.push("../pages/OtherSettings.qml")
            popupLoader.state="show2"
            inputPanel.state="show"
             pascom.enabled=true
            cancelButton.state="idle"
            holdState()
        }
        Connections{
            id:othcom
            enabled:false
            target: popupLoader.item
            ignoreUnknownSignals: true
            onAccepted:{
                recoverState()

                    console.log("othxcom")
               othcom.enabled=false
            }
            onCanceled:{
                recoverState()
                 othcom.enabled=false
            }
        }
    }

    Connections {
        id: acceptAction01
        target: acceptButton
        enabled: false
        onButtonClicked: {            
            accepted()

            acceptAction01.enabled=false

        }
    }
    Component.onCompleted: {

        acceptButton.pushRole([ acceptAction01,qsTr("OK"),"idle"])

    }
    Component.onDestroyed: {
        acceptButton.popRole()
    }
}
