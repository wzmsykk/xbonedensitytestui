import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle {
    id: cancelButton               
    color: "transparent"

    implicitHeight: 40
    implicitWidth: 60
    property alias text: label1.text
    Behavior on opacity {NumberAnimation{}}
    state: "invisible"
    signal buttonClicked()
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    property var rolestack: [[null,qsTr("OK"),"invisible"]]
    property int index: 0
    //var role=[connection name,textString]
    function pushRole(inRole){
        if(index>0&&rolestack[index][0]!==null)rolestack[index][0].enabled=false
        rolestack.push(inRole)
        index++
        if(rolestack[index][1]!==null) text=rolestack[index][1]
        if(rolestack[index][0]!==null) rolestack[index][0].enabled=true
        console.log(rolestack[index])
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

    states: [
        State {
            name: "idle"
            PropertyChanges {
                target: cancelButton
                visible:true
                opacity:1

            }
            PropertyChanges {
                target: backtangle
                color:"#607D8B"
                opacity:0.6
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity:1
            }
        },State {
            name: "pressed"
            PropertyChanges {
                target: backtangle
                color:"#F44336"
                opacity:0.4
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity: 1
            }
        },State {
            name: "invisible"
            PropertyChanges {
                target: cancelButton
                visible:false
                opacity:0

            }
        }]
    Rectangle{
        id:backtangle
        anchors.fill: parent
        color: "#607D8B"
        opacity: 0.8
    }
    Label{
        id:label1
        text: qsTr("Cancel")
        color: "white"
        anchors.fill: cancelButton
        anchors.margins: 8
        fontSizeMode: Text.VerticalFit
        minimumPixelSize: 10
        font.pixelSize: 72
        opacity: 0.8
    }
    
    MouseArea{
        id:cancelAction
        anchors.fill: parent
        enabled: parent.visible
        onPressed: {
            cancelButton.state="pressed"
        }
        onClicked: {
            buttonClicked()
            mouse.accepted=true
        }
        onReleased: {
            cancelButton.state="idle"
        }
    }
    
}
