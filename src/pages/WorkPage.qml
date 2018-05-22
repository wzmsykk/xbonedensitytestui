import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import com.kp.scan 1.0
import "../components"
FuzzyPanel {
    id: workPage
    anchors.fill: parent
    target: backGImage
    property string state: "idle"

    signal canceled()
    signal accepted()
    Scan {
        id: scan

        onHandleScanPreperationResults: {
            if (result === 1) {
                wplb.text = qsTr("prepared")
                acceptButton.text=qsTr("Scan")
                state="prepared"
            }
        }
        onHandleScanResults: {
            if (result === 1) {
                wplb.text = qsTr("scanFinished")
            } else if (result === 2) {
                wplb.text = qsTr("doing sth")
            } else if (result === 3) {
                wplb.text = qsTr("allfinished,press Next to show result")

            } else if (result === 4) {
                resultReady=true
                state="resultReady"
                acceptButton.text=qsTr("Next")
                acceptButton.state="idle"
                 acceptAction00_w.enabled = true

            }
        }
    }

    Text {
        id: wplb
        color: primaryColor
        width: parent.width
        text: ""
        anchors.verticalCenter: parent.verticalCenter
        fontSizeMode: Text.Fit
        font.pixelSize: 22
        wrapMode: Text.Wrap
    }
    Connections {
        id: backAction00_w
        target: cancelButton
        enabled: false
        onButtonClicked: {

            //dosth to stop test
            canceled()

        }
    }

    Connections {
        id: acceptAction00_w
        target: acceptButton
        enabled:true
        onButtonClicked: {
            //do sth to start ScanProgress
            if(state==="prepared"){
            scan.operateScan()
            acceptButton.text=qsTr("Wait")
            acceptButton.state="invalid"
                 acceptAction00_w.enabled = false
            }
            else if(state==="resultReady"){
                accepted()
            }



        }
    }

    Component.onCompleted: {

        acceptButton.pushRole([acceptAction00_w,qsTr("Wait"),"idle"])
        cancelButton.pushRole([backAction00_w,qsTr("cancel"),"idle"])
        scan.prepareScan()
    }
    Component.onDestruction: {
       acceptButton.popRole()
        cancelButton.popRole()
    }
}
