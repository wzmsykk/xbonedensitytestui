import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import com.kp.scan 1.0

FuzzyPanel {
    id: workPage
    anchors.fill: parent
    target: backGImage
    function setButtonRoles(){

    }

    signal canceled()
    signal accepted()
    Scan {
        id: scan

        onHandleScanPreperationResults: {
            if (result === 1) {
                wplb.text = qsTr("prepared")
                acceptButton.text=qsTr("Scan")
                acceptAction00_w.enabled=true
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
                acceptAction01_w.enabled=true
                acceptButton.text=qsTr("Next")
                acceptButton.state="idle"

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
            backAction00_w = enabled = false
            acceptAction00_w.enabled = false
        }
    }

    Connections {
        id: acceptAction00_w
        target: acceptButton
        enabled: false
        onButtonClicked: {
            //do sth to start ScanProgress
            scan.operateScan()
            acceptButton.text=qsTr("Wait")
            acceptButton.state="invalid"
            acceptAction00_w.enabled = false
            backAction00_w = enabled = false
        }
    }
    Connections {
        id: acceptAction01_w
        target: acceptButton
        enabled: false
        onButtonClicked: {

             accepted()
            acceptAction01_w.enabled = false
            backAction00_w = enabled = false
        }
    }
    Component.onCompleted: {
        acceptButton.text=qsTr("Wait")
        backAction00_w.enabled = true
        acceptAction00_w.enabled = true
        scan.prepareScan()
    }
    Component.onDestruction: {
        acceptAction00_w.enabled = false
        backAction00_w.enabled = false
    }
}
