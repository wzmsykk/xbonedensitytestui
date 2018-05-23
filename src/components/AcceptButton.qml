import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle {
    id: acceptButton
    property var bText
    color: "transparent"
    implicitHeight: 40
    implicitWidth: 60
    state: "invisible"
    property alias text: label1.text
    signal buttonClicked()
    property var rolestack: [[null,qsTr("OK"),"invisible"]]
    property int index: 0
    //var role=[connection name,textString]
    function pushRole(inRole){
        if(index>0&&rolestack[index][0]!==null)rolestack[index][0].enabled=false
        rolestack.push(inRole)
        console.log(inRole)
        index++
        if(rolestack[index][1]!==null) text=rolestack[index][1]
         if(rolestack[index][0]!==null) rolestack[index][0].enabled=true

         if(rolestack[index].length===3) state=rolestack[index][2]
         return 1
    }
   function popRole(){
       if(index===0)
       return 0
       else {
       if(rolestack[index][0]!==null) rolestack[index][0].enabled=false
       rolestack.pop()
        index--
       if(rolestack[index][1]!==null) text=rolestack[index][1]
        if(index>=1&&rolestack[index][0]!==null) rolestack[index][0].enabled=true
           if(rolestack[index].length===3) state=rolestack[index][2]

       }
       return 1
   }

    Behavior on opacity {NumberAnimation{}}
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    states: [
        State {
            name: "idle"
            StateChangeScript{
                script: {
                    console.log("AcceptBS:idle")
                }
            }

            PropertyChanges {
                target: acceptButton
                visible:true
                opacity:1
                enabled:true

            }
            PropertyChanges{
                target: acceptAction
                enabled:true
            }
            PropertyChanges {
                target: backtangle
                color:"#4CAF50"
                opacity:1
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity: 1
            }
        },State {
            name: "pressed"
            PropertyChanges {
                target: backtangle
                color:"#9E9E9E"
                opacity:0.8
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity: 1
            }
        },State {
            name: "invisible"
            StateChangeScript{
                script: {
                    console.log("AcceptBS:invisible")
                }
            }

            PropertyChanges {
                target: acceptButton
                visible:false
                enabled:false
                opacity:0

            }
            PropertyChanges{
                target: acceptAction
                enabled:false
            }
        },State {
            name: "invalid"
            PropertyChanges{
                target: acceptAction
                enabled:false            
            }
            PropertyChanges {
                target: backtangle
                color:"#9E9E9E"
                opacity:0.8
            }
        }]
    Rectangle{
        id:backtangle
        anchors.fill: parent
        color: parent.enabled?"#607D8B":"grey"
        opacity: 0.8
    }
    Label{
        id:label1
        text: qsTr("Accept")
        color: "white"
        anchors.fill: acceptButton
        anchors.margins: 8
        fontSizeMode: Text.VerticalFit
        minimumPixelSize: 10
        font.pixelSize: 72
        opacity: 0.8
    }

    MouseArea{
        id:acceptAction
        anchors.fill: parent
        enabled: true
        onPressed: {
            acceptButton.state="pressed"
        }
        onClicked: {
            buttonClicked()
            mouse.accepted=true
        }
        onReleased: {
            acceptButton.state="idle"
        }
    }
}
