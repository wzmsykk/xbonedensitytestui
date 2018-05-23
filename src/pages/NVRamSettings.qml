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

        transitions: Transition {
            AnchorAnimation {
                duration: 20000
            }
        }

        FlatSelection {
            id: ps01
            title: qsTr("CalibratePMT on next power up")
            focus: true
            model: [qsTr("Yes"), qsTr("No")]
            width: (parent.width - parent.spacing * 3) / 2
            height: (parent.height - parent.spacing * 5) / 3
            onFocused: {
                inputPanel.targetDist = ps01
                inputPanel.role = "selection"
                tmchildSeq.activeIndex = 0
            }
        }
        FlatTextField {
            id: ps02
            title: qsTr("Gain")
            //model: ["colored","grey"]
            width: (parent.width - parent.spacing * 3) / 2
            height: (parent.height - parent.spacing * 5) / 3
            onFocused: {
                inputPanel.targetDist = ps02
                inputPanel.role = "numberial"
                tmchildSeq.activeIndex = 1
            }
            validator: IntValidator {
                bottom: 0
                top: 100000
            }
        }
        FlatTextField {
            id: ps03
            title: qsTr("Offset")
            //model: ["colored","grey"]
            width: (parent.width - parent.spacing * 3) / 2
            height: (parent.height - parent.spacing * 5) / 3
            onFocused: {
                inputPanel.targetDist = ps03
                inputPanel.role = "numberial"
                tmchildSeq.activeIndex = 2
            }
            validator: IntValidator {
                bottom: -66
                top: 66
            }
        }
        FlatTextField {
            id: ps04
            title: qsTr("Avg.Mu")
            //model: ["colored","grey"]
            width: (parent.width - parent.spacing * 3) / 2
            height: (parent.height - parent.spacing * 5) / 3
            onFocused: {
                inputPanel.targetDist = ps04
                inputPanel.role = "numberial"
                tmchildSeq.activeIndex = 3
            }
            validator: IntValidator {
                bottom: 0
                top: 23
            }
        }
        FlatTextField {
            id: ps05
            title: qsTr("Serial")
            //model: ["colored","grey"]
            width: (parent.width - parent.spacing * 3) / 2
            height: (parent.height - parent.spacing * 5) / 3
            onFocused: {
                inputPanel.targetDist = ps05
                inputPanel.role = "numberial"
                tmchildSeq.activeIndex = 4
            }
            validator: IntValidator {
                bottom: 0
                top: 9999999
            }
        }

        Item {
            id: tmchildSeq
            property var seq: [ps01, ps02, ps03, ps04, ps05]
            property int activeIndex: 0
            onActiveIndexChanged: {
                if (activeIndex === seq.length - 1) {
                    acceptButton.text = qsTr("Commit")
                    cancelButton.text = qsTr("Previous")
                } else if (activeIndex === 0) {
                    acceptButton.text = qsTr("Next")
                    cancelButton.text = qsTr("Cancel")
                } else {
                    cancelButton.text = qsTr("Previous")
                    acceptButton.text = qsTr("Next")
                }
            }

            function next(index) {
                if (index < seq.length - 1) {
                    seq[index + 1].forceFocus()
                    activeIndex = index + 1
                    console.log(activeIndex)
                } else
                    commit()
            }
            function previous(index) {
                if (index === 0) {
                    activeIndex = 0
                    cancelButton.text = qsTr("Cancel")
                    acceptButton.text = qsTr("OK")

                    accept.enabled = false
                    cancel.enabled = false
                    inputPanel.state = "hide"
                    inputPanel.setDefaultDist()
                    canceled()
                } else {
                    seq[index - 1].forceFocus()
                    activeIndex = index - 1
                }
            }

            function commit() {
                for (var i = 0; i < 5; i++)
                    console.log(seq[i].model[0])
                activeIndex = 0
                inputPanel.setDefaultDist()
                accept.enabled = false
                cancel.enabled = false
                accepted()
            }
        }
    }
    Component.onCompleted: {

        ps01.forceFocus()
        acceptButton.pushRole([accept, qsTr("Next"), "idle"])
        cancelButton.pushRole([cancel, qsTr("Cancel"), "idle"])
        inputPanel.role = "numberial"
        inputPanel.targetDist = ps01
    }
    Component.onDestruction: {
        inputPanel.setDefaultDist()
        acceptButton.popRole()
        cancelButton.popRole()
    }

    Connections {
        id: accept
        target: acceptButton
        enabled: true
        onButtonClicked: {

            tmchildSeq.next(tmchildSeq.activeIndex)
        }
    }
    Connections {
        id: cancel
        target: cancelButton
        enabled: true
        onButtonClicked: {

            tmchildSeq.previous(tmchildSeq.activeIndex)
        }
    }
}
