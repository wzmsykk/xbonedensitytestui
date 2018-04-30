import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Item{
    function showActions00(){
        acceptButton.state="idle"
        cancelButton.state="idle"
        lefttopContentArea.enabled=true
        lefttopContentArea.visible=true
        lefttopInputPanelShowAction.enabled=true
        backAction00.enabled=true
    }
    function hideActions00(){
        acceptButton.state="invisible"
        cancelButton.state="invisible"
        lefttopContentArea.enabled=false
        lefttopContentArea.visible=false
        backAction00.enabled=false
    }
    id:anchorScript
    transitions: Transition {
        AnchorAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    state: "normal"
    states: [
        State {
            name: "normal"
            AnchorChanges{
                target: inputPanel
                anchors.right: undefined
                anchors.left: parent.right
            }
            StateChangeScript{
                script: {
                    acceptButton.state="invisible"
                    cancelButton.state="invisible"
                    lefttopContentArea.enabled=false
                    lefttopContentArea.visible=false
                    inputPanel.state="hide"}
            }
            
        },State {
            name: "lefttopshow"
            StateChangeScript{
                script: showActions00()
            }
            AnchorChanges{
                target: inputPanel
                anchors.right: undefined
                anchors.left: parent.right
                
            }
            AnchorChanges {
                target: lefttop.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges {
                target: leftbottom
                anchors.bottom: undefined
                anchors.left: undefined
                anchors.top: parent.bottom
                anchors.right: parent.right
            }
            AnchorChanges {
                target: righttop
                anchors.top: undefined
                anchors.right: undefined
                anchors.bottom: parent.bottom
                anchors.left: parent.right
            }
            AnchorChanges {
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.bottom
                anchors.left: parent.right
            }
            
            AnchorChanges {
                target: lefttop
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: toolbar.top
                anchors.right: parent.right
            }
            PropertyChanges {
                target: lefttop
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin
                anchors.rightMargin: vMargin
            }
        },State {
            name: "lefttopInput"
            extend: "lefttopshow"
            AnchorChanges{
                target: lefttop
                anchors.right: inputPanel.left
            }
            AnchorChanges{
                target: inputPanel
                anchors.left: undefined
                anchors.right: parent.right
            }
            
            }]
}
