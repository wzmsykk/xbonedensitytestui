import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Grid{
    anchors.fill: parent
    columns: 1
    rows:3
    spacing: 6
    padding: 6
    signal accepted(var result)
    signal canceled()
    function getFingerVisibles(flag){
        ps01.index=flag%2
        flag/=2
        ps02.index=flag%2
        flag/=2
        ps03.index=flag%2
    }
    function generateFingerVisibles(){
        return ps01.index*4+ps02.index*2+ps03.index*1
    }

    FlatSelection{
        id:ps01
        title: qsTr("Finger#1")
        model:[qsTr("unselected"),qsTr("selected")]
        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps01
        }
    }
     FlatSelection{
        id:ps02
        title: qsTr("Finger#2")
         model:[qsTr("unselected"),qsTr("selected")]
        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps02
        }
    }
     FlatSelection{
         id:ps03
         title: qsTr("Finger#3")
         model:[qsTr("unselected"),qsTr("selected")]
         width: parent.width-2*padding
         onFocused: {
             inputPanel.targetDist=ps03
         }
     }

    Component.onCompleted: {
        inputPanel.role="selection"
          inputPanel.targetDist=ps01
            ps01.forceActiveFocus()
          cancelButton.state="idle"

    }
    function recover(){
        inputPanel.setDefaultDist();
    }

    Component.onDestruction: {

    }

    Connections{
        id:accept
        target: acceptButton
        enabled:true
        onButtonClicked:{
                recover();
              accepted(generateFingerVisibles())
            accept.enabled=false
        }
    }
    Connections{
        id:cancel
        target: cancelButton
        enabled:true
        onButtonClicked:{
            recover();
            canceled()
            cancel.enabled=false
        }
    }


}
