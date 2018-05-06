import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Item {
    id: anchorScript
    transitions: Transition {
        AnchorAnimation{duration: 200}
    }
    state: "normal"
    states: [
        State {
            name: "normal"
            extend: ""

            AnchorChanges {
                target: inputPanel
                anchors.right: undefined
                anchors.left: parent.right
            }


            StateChangeScript {
                script: {
                    acceptButton.state = "invisible"
                    cancelButton.state = "invisible"
                    inputPanel.state = "hide"
                    lefttopContentArea.state="01"
                    lefttopShowAction.enabled=true
                    leftbottomShowAction.enabled=true
                    righttopShowAction.enabled=true
                    rightbottomShowAction.enabled=true
                    console.log("stateNorm")
                }
            }
        },
        State {
            name: "lefttopshow"
            extend: "normal"
            StateChangeScript {
                script: {
                    //lefttopInputPanelShowAction.enabled = true


                    //ltchildSeq.seq[0].forceFocus()
                    acceptButton.text="Next"
                    console.log("ltshow")
                }
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
            PropertyChanges {
                target: acceptButton
                state: "idle"
            }
            PropertyChanges {
                target: cancelButton
                state: "idle"
            }
            PropertyChanges {
                target: lefttopContentArea
                state:"02"

            }

        },
        State {
            name: "lefttopInput"
            extend: "lefttopshow"
            StateChangeScript{
                script: {

                    console.log("ltinp")
                }
            }



            AnchorChanges {
                target: inputPanel
                anchors.left: undefined
                anchors.right: parent.right

            }
            AnchorChanges {
                target: lefttop
                anchors.top:parent.top
                anchors.left: parent.left
                anchors.right: inputPanel.left
                anchors.bottom: toolbar.top
            }


        },
        State {
            name: "work"
            extend: "lefttopshow"

        },
        State {
            name: "leftbottomshow"
            extend: "normal"
            StateChangeScript {
                script: {
                    backAction10.enabled = true
                    acceptAction10.enabled=true
                }
            }

            AnchorChanges {
                target: leftbottom.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            PropertyChanges {
                target: leftbottom
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin
                anchors.rightMargin: vMargin
            }
            AnchorChanges {
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom: parent.top
                anchors.right: parent.right
            }
            AnchorChanges {
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.top
                anchors.left: parent.right
            }
            AnchorChanges {
                target: leftbottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: toolbar.top

                anchors.right: parent.right
            }
            AnchorChanges {
                target: righttop
                anchors.right: undefined
                anchors.top: undefined
                anchors.bottom: parent.top

                anchors.left: parent.right
            }
            PropertyChanges {
                target: cancelButton
                enabled: true
                state: "idle"
            }
            PropertyChanges {
                target: acceptButton
                enabled: true
                state: "idle"
            }
            PropertyChanges {
                target: leftbottomContentArea
                state:"02"

            }
        },
        State {
            name: "righttopshow"
            extend: "normal"
            StateChangeScript {
                script: {
                    acceptAction01.enabled = true
                }
            }
            PropertyChanges{
                target: acceptButton
                enabled:true
                text:qsTr("OK")
                state:"idle"
            }
            PropertyChanges {
                target: righttop
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin
                anchors.rightMargin: vMargin
            }
            AnchorChanges {
                target: righttop.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges {
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom: parent.bottom
                anchors.right: parent.left
            }
            AnchorChanges {
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.bottom
                anchors.left: parent.left
            }
            AnchorChanges {
                target: leftbottom
                anchors.left: undefined

                anchors.bottom: undefined
                anchors.top: parent.bottom
                anchors.right: parent.left
            }
            AnchorChanges {
                target: righttop
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: toolbar.top
                anchors.right: parent.right
            }
            PropertyChanges{
                target: righttopContentArea
                state:"02"
            }

        },
        State {
            name: "rightbottomshow"
            extend: "normal"
            StateChangeScript {
                script: {
                    acceptAction11.enabled = true
                }
            }
            PropertyChanges{
                target: acceptButton
                enabled:true
                text:qsTr("OK")
                state:"idle"
            }
            PropertyChanges {
                target: rightbottom
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin
                anchors.rightMargin: vMargin
            }
            AnchorChanges {
                target: rightbottom.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges {
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom: parent.top
                anchors.right: parent.left
            }
            AnchorChanges {
                target: righttop
                anchors.bottom: parent.top
                anchors.right: undefined
                anchors.top: undefined
                anchors.left: parent.left
            }
            AnchorChanges {
                target: leftbottom
                anchors.left: undefined
                anchors.top: parent.top
                anchors.bottom: undefined

                anchors.right: parent.left
            }
            AnchorChanges {
                target: rightbottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: toolbar.top
                anchors.right: parent.right
            }
            PropertyChanges {
                target: rightbottomContentArea
                state:"02"

            }
        }
    ]



    Connections {
        id: backAction01
        target: cancelButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            backAction01.enabled=false
            righttopShowAction.enabled = true
        }
    }
    Connections {
        id: acceptAction01
        target: acceptButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            acceptAction01.enabled=false
            righttopShowAction.enabled = true
        }
    }
    Connections {
        id: acceptAction10
        target: acceptButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            acceptAction10.enabled=false
            backAction10.enabled=false
            leftbottomShowAction.enabled = true
        }
    }
    Connections {
        id: backAction10
        target: cancelButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            backAction10.enabled=false
            acceptAction10.enabled=false
            leftbottomShowAction.enabled = true
        }
    }
    Connections {
        id: backAction11
        target: cancelButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            acceptAction11.enabled=false
            backAction11.enabled=false
            rightbottomShowAction.enabled = true
        }
    }
    Connections {
        id: acceptAction11
        target: acceptButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            acceptAction11.enabled=false
            backAction11.enabled=false
            rightbottomShowAction.enabled = true
        }
    }
    Connections {
        id: lefttopShowAction
        target: lefttop
        enabled: true
        onPanelClicked: {
            anchorScript.state="lefttopshow"
            anchorScript.state = "lefttopInput"

            lefttopShowAction.enabled = false
            //lefttopInputPanelShowAction = true
        }
    }
    Connections {
        id: lefttopInputPanelShowAction
        target: lefttop
        enabled: false
        onPanelClicked: {

            anchorScript.state = "lefttopInput"
            lefttopInputPanelShowAction.enabled=false
        }
    }
    Connections {
        id: leftbottomShowAction
        target: leftbottom
        enabled: true
        onPanelClicked: {
            anchorScript.state = "leftbottomshow"
            leftbottomShowAction.enabled = false
        }
    }
    Connections {
        id: righttopShowAction
        target: righttop
        enabled: true
        onPanelClicked: {
            anchorScript.state = "righttopshow"
            righttopShowAction.enabled = false
        }
    }
    Connections {
        id: rightbottomShowAction
        target: rightbottom
        enabled: true
        onPanelClicked: {
            anchorScript.state = "rightbottomshow"
            rightbottomShowAction.enabled = false
        }
    }

}
