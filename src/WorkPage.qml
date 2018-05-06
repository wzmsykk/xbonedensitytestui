import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import com.kp.scan 1.0

FuzzyPanel {
    id: workPage
    anchors.fill: parent
    target: backGImage

    signal canceled
    signal accepted
    Scan {
        id: scan

        onHandleScanPreperationResults: {
            if (result === 1) {
                wplb.text = "prepared"
                scan.operateScan()
            }
        }
        onHandleScanResults: {
            if (result === 1) {
                wplb.text = "scanFinished"
            } else if (result === 2) {
                wplb.text = "doing sth"
            } else if (result === 3) {
                wplb.text = "allfinished"
            } else if (result === 4) {

                accepted()
            }
        }
    }

    Text {
        id: wplb
        color: primaryColor
        text: "Start"
        anchors.verticalCenter: parent.verticalCenter
        fontSizeMode: Text.Fit
        font.pixelSize: 72
    }
    Connections {
        id: backAction00_w
        target: cancelButton
        enabled: false
        onButtonClicked: {

            //dosth to stop test
            canceled()

            modalPopupLoader.state = "hide"
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
            acceptAction00_w.enabled = false
            backAction00_w = enabled = false
        }
    }
    Component.onCompleted: {

        backAction00_w.enabled = true
        acceptAction00_w.enabled = true
        scan.prepareScan()
    }
    Component.onDestruction: {
        acceptAction00_w.enabled = false
        backAction00_w.enabled = false
    }
}
