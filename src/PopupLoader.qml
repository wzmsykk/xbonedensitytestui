import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0
Loader{
    id:popupLoader
    width: parent.width-2*hMargin
    anchors.leftMargin: hMargin
    anchors.topMargin: vMargin
    anchors.bottomMargin: hMargin
    anchors.rightMargin: vMargin

    state:"hide"
    states: [State {
            name: "hide"
            PropertyChanges{
                target: popupLoader
                opacity:0
            }
            AnchorChanges {
                target: popupLoader
                anchors.left:parent.right
                anchors.right:  undefined
            }
        },State {
            name: "show"
            AnchorChanges {
                target: popupLoader
                anchors.left: parent.left
                anchors.right: parent.right
            }
            PropertyChanges{
                target: popupLoader
                opacity:1
            }
        },State {
            name: "show2"
            AnchorChanges {
                target: popupLoader
                anchors.left: parent.left
                anchors.right: inputPanel.left
            }
            PropertyChanges{
                target: popupLoader
                opacity:1
            }
        }]
    Rectangle{
        visible: false
        anchors.fill: parent
        border.color: "green"
    }

    Connections{
        target: popupLoader.item
        ignoreUnknownSignals: true
        onCanceled:{
            popupLoader.state="hide"
            popupLoader.source=""
        }
        onAccepted:{
            popupLoader.state="hide"
            //anchorScript.state="leftbottomshow"
            popupLoader.source=""
        }
    }



    transitions: Transition {
        AnchorAnimation{
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on opacity {
        NumberAnimation{}
    }
    

    
}

