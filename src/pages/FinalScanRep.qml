import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import "../components"
import com.kp.scan 1.0

FuzzyPanel {
    anchors.fill: parent
    implicitHeight: 480
    implicitWidth: 600
    signal accepted
    signal canceled
    target: backGImage

    Scan {
        id: myScan
        property bool onProc: false
        property bool intR: false
        property int scanCount: 0
        property int maxScan: 0
        function startRegularScan() {
            ps03.editable=false
            myScan.maxScan = ps03.content.text * 1
            myScan.prepareScan()
            scanCount=0
            ps04.content.text=scanCount.toString()
            intR=false
            onProc=true
            acceptButton.text = qsTr("Wait")
            acceptButton.state = "invalid"
            cancelButton.text=qsTr("Stop")
        }
        function nextStep() {
            scanCount++
            ps04.content.text=scanCount.toString()
            if (scanCount < maxScan && intR!==true) {
                myScan.prepareScan()
                onProc=true
            }else {
                ps05.content.text = qsTr("End")
                acceptButton.text = qsTr("Start")
                acceptButton.state = "idle"
                onProc=false
                cancelButton.text = qsTr("Cancel")
                cancelButton.state = "idle"
                ps03.editable=true
            }
        }
        function stop(){
            intR=true

        }

        onHandleScanPreperationResults: {
            if (result === 1) {
                ps05.content.text = qsTr("prepared")
                myScan.operateScan()
            } else {
                ps05.content.text = qsTr("ErrorCode:%1").arg(result)
            }
        }
        onHandleScanResults: {
            if (result === 1) {
                ps05.content.text = qsTr("exposeOK")
            } else if (result === 2) {
                ps05.content.text = qsTr("Calulate")
            } else if (result === 3) {
                ps05.content.text = qsTr("GenImage")
            } else if (result === 5) {
                ps05.content.text = qsTr("ScanOK")
                nextStep()
            } else if (result >5){ ps05.content.text = qsTr("ErrorCode:%1"). arg(result)}
        }
    }

    Column {
        id: lt02
        anchors.fill: parent
        anchors.margins: 20
        padding: 6
        spacing: 6
        Label {
            text: qsTr("Final Scan with Report")
        }
        FlatTextField {
            id: ps03
            title: qsTr("Max Scan Allowed")
            validator: IntValidator {
                bottom: 0
                top: 65535
            }

            width: parent.width - 2 * parent.padding
            onFocused: {
                inputPanel.targetDist = ps03
                inputPanel.role = "numberial"
            }
        }
        FlatTextField {
            id: ps04
            editable: false
            title: qsTr("Number of Scan")
            content.text: "0"
            width: parent.width - 2 *  parent.padding
        }
        FlatTextField {
            id: ps05
            editable: false
            title: qsTr("Status")
            content.text: qsTr("Idle")
            width: parent.width - 2 *  parent.padding
        }
    }
    Connections {
        id: accept
        target: acceptButton
        enabled: true
        onButtonClicked: {

            myScan.startRegularScan()


            //accepted()
        }
    }
    Connections {
        id: cancel
        target: cancelButton
        enabled: true
        onButtonClicked: {
            if(myScan.onProc===true){
                myScan.stop()
                cancelButton.text=qsTr("Stopping")
                cancelButton.state="invalid"
            }else{
            canceled()
            }
        }
    }

    Component.onCompleted: {
        ps03.forceFocus()
        acceptButton.pushRole([accept, qsTr("Start"), "idle"])
        cancelButton.pushRole([cancel, qsTr("Cancel"), "idle"])
        // thirdButton.pushRole([cancel,qsTr("Cancel"),"idle"])
    }
    Component.onDestruction: {
        inputPanel.setDefaultDist()
        acceptButton.popRole()
        cancelButton.popRole()
        // thirdButton.popRole()
    }
}
