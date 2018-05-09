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
    signal accepted()
    signal canceled()
    FlatSelection{
        id:ps01
        title: qsTr("Language")
        model: ["en","zh"]
        index:0
        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps01
            inputPanel.role="selection"
        }
    }
    FlatSelection{
        id:ps02
        title: qsTr("Test UI Style")
         index:0
        model: ["disabled","enabled"]
        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps02
            inputPanel.role="selection"
        }

    }
    FlatTextField{
        id:ps03
        title: qsTr("code")
        validator: IntValidator{
            bottom: 0
            top:999999
        }

        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps03
            inputPanel.role="numberial"

        }
    }
    function loadOtherSettings(){
         if(style==="simple") {ps02.index=0}else{ps02.index=1}

    }

    Component.onCompleted: {
            inputPanel.role="selection"
            ps01.forceFocus()
          cancelButton.state="idle"


    }
    Connections{
        id:accept
        target: acceptButton
        enabled:true
        onButtonClicked:{
           if(ps02.index===0) {style="simple"} else {style="test"}
              accepted()
            accept.enabled=false
        }
    }
    Connections{
        id:cancel
        target: cancelButton
        enabled:true
        onButtonClicked:{
            canceled()
            cancel.enabled=false
        }
    }


}
