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
    columns: 2
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
    Settings{
        id:timeSet
    }

    FlatTextField {
        id: ps01
        title: qsTr("Year")
        focus: true

        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onFocused: {
            inputPanel.targetDist = ps01
            inputPanel.role = "numberial"
            tmchildSeq.activeIndex = 0
        }
        validator: IntValidator {
            bottom: 0
            top: 2050
        }
    }
    FlatTextField {
        id: ps02
        title: qsTr("Month")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onFocused: {
            inputPanel.targetDist = ps02
            inputPanel.role = "numberial"
            tmchildSeq.activeIndex = 1
        }
        validator: IntValidator {
            bottom: 1
            top: 12
        }
    }
    FlatTextField {
        id: ps03
        title: qsTr("Day")
        //model: ["colored","grey"]
        width: (parent.width - parent.spacing * 3) / 2
        height: (parent.height - parent.spacing * 5) / 3
        onFocused: {
            inputPanel.targetDist = ps03
            inputPanel.role = "numberial"
            tmchildSeq.activeIndex = 2
        }
        validator: IntValidator {
            bottom: 1
            top: 31
        }
    }
    FlatTextField {
        id: ps04
        title: qsTr("Hour")
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
        title: qsTr("Minute")
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
            top: 59
        }
    }

function setTimeText(){
    var tmp=new Date()
    console.log(tmp)
    console.log(tmp.getFullYear())
   ps01.content.text=tmp.getFullYear()
    ps02.content.text=tmp.getMonth()
    ps03.content.text=tmp.getDay()
    ps04.content.text=tmp.getHours()
    ps05.content.text=tmp.getMinutes()
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
                acceptButton.text=qsTr("OK")

                accept.enabled = false
                cancel.enabled = false
                 inputPanel.state="hide"
                inputPanel.setDefaultDist()
                canceled()
            } else {
                seq[index - 1].forceFocus()
                activeIndex = index - 1
            }
        }

        function commit() {
            for(var i=0;i<5;i++) console.log(seq[i].model[0])
            activeIndex = 0
            inputPanel.state="hide"
            inputPanel.setDefaultDist()
            accept.enabled = false
            cancel.enabled = false
            acceptButton.text=qsTr("OK")
             var newdate=new Date(seq[0].model[0]*1,seq[1].model[0]*1-1,seq[2].model[0]*1,seq[3].model[0]*1,seq[4].model[0]*1)

            console.log(newdate)
            timeSet.changeTime(newdate)
            accepted()
        }
    }
    Component.onCompleted: {
        setTimeText()
        ps01.forceFocus()

        inputPanel.role = "numberial"
        inputPanel.targetDist = ps01
        acceptButton.pushRole([accept,qsTr("Next"),"idle"])
        cancelButton.pushRole([cancel, qsTr("Cancel"),"idle"])

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
