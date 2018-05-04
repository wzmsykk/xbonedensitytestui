import QtQuick 2.7
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import com.kp.settings 1.0
import QtQuick.Controls.Material 2.3

Page {
    id: page
    title: qsTr("Preferences")
    property int cardWidth: (page.width - FluidControls.Units.smallSpacing * 8) / 3
    ListModel {
        id: list1
    }
    ListModel {
        id: list2
        ListElement {
            key: "Grey"
            value: false
        }
        ListElement {
            key: "Colored"
            value: true
        }
    }

    Row {
        anchors.top: parent.top
        topPadding: FluidControls.Units.smallSpacing * 2
        leftPadding: FluidControls.Units.smallSpacing * 2
        spacing: FluidControls.Units.smallSpacing * 2

        Card1 {
            id: card1
            width: cardWidth
            height: 0.9*page.height
            titleText: qsTr("SetPassWord")
            bodyText: qsTr("SetPassWord")
            focus: true


        }
        Card1 {
            id: card2
            width: cardWidth
            height: 0.9*page.height
            titleText: qsTr("SetTime")
            bodyText: qsTr("SetTime")


        }
        Card1 {
            id: card3
            width: cardWidth
            height: 0.9*page.height
            titleText: qsTr("PrintSettings")
            bodyText: qsTr("PrintSettings")


        }
        Connections {
            target: card1
            onPressed: {
                mySet.readSettings()
                if (mySet.isPswExist === false) {
                    passDialog.oldVisible = false
                } else {
                    passDialog.oldVisible = true
                }
                passDialog.open()
                passDialog.focus=true
            }
        }
        Connections {
            target: card2
            onPressed: {
                timeD.open()
                timeD.focus=true
            }
        }
        Connections {
            target: card3
            onPressed: {
                printSetDialog.open()
                printSetDialog.focus=true
            }
        }
    }
    FluidControls.DateTimePickerDialog {
        id: timeD
        Material.elevation: 24
        onAccepted: {
            console.log(timeD.selectedDateTime)
            var err=mySet.changeTime(timeD.selectedDateTime)
            console.log(err)
            if(err===-1) snackBar.open(qsTr("Need Previlige to Change Time."))
            else snackBar.open(qsTr("Time Changed."))
        }
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
    }

    PassDialog {
        id: passDialog
    }
    PrintSetDialog {
        id: printSetDialog
    }
}
