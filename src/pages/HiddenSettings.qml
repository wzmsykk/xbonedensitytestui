import QtQuick 2.7

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
    target: backGImage
    ListModel {
        id: myl
    }
    function recoverState() {
        popupLoader.pop()
    }
    function holdState() {

    }

    function genInfoList() {
        myl.clear()

        myl.append({
                       name: qsTr("ByteUsed"),
                       num: mySet.byteUsed.toString()
                   })
        myl.append({
                       name: qsTr("CheckSum"),
                       num: mySet.checkSum.toString()
                   })
        myl.append({
                       name: qsTr("PMTValue"),
                       num: mySet.PMTValue.toString()
                   })
        myl.append({
                       name: qsTr("T-offset"),
                       num: mySet.toffset.toString()
                   })
        myl.append({
                       name: qsTr("Z-offset"),
                       num: mySet.zoffset.toString()
                   })
        myl.append({
                       name: qsTr("Gain"),
                       num: mySet.gain.toString()
                   })
        myl.append({
                       name: qsTr("Offset"),
                       num: mySet.offset.toString()
                   })
        myl.append({
                       name: qsTr("AvgMu"),
                       num: mySet.avgmu.toString()
                   })
        myl.append({
                       name: qsTr("SerialNum."),
                       num: mySet.serial.toString()
                   })
        myl.append({
                       name: qsTr("S/W Ver."),
                       num: mySet.sver.toString()
                   })
        myl.append({
                       name: qsTr("DB Ver."),
                       num: mySet.dver.toString()
                   })
        myl.append({
                       name: qsTr("BMD #1"),
                       num: mySet.bmd1.toString()
                   })
        myl.append({
                       name: qsTr("BMD #2"),
                       num: mySet.bmd2.toString()
                   })
        myl.append({
                       name: qsTr("BMD #3"),
                       num: mySet.bmd3.toString()
                   })
        myl.append({
                       name: qsTr("SNR"),
                       num: mySet.snr.toString()
                   })
        myl.append({
                       name: qsTr("Mu"),
                       num: mySet.mu.toString()
                   })
    }

    Grid {
        id: gr1
        width: parent.width * 0.6
        height: parent.height
        spacing: 6
        columns: 2
        rows: 8
        flow: Grid.TopToBottom
        property var customData: ListModel {
            ListElement {
                name: qsTr("Byte")
            }
        }

        property int childWidth: (width - (spacing + 2) * columns) / columns
        property int childHeight: (height - (spacing + 2) * rows) / rows
        Repeater {
            model: myl
            Text {
                id: devText
                width: gr1.childWidth
                height: gr1.childHeight
                text: name + ":" + num
                fontSizeMode: Text.Fit
            }
        }
    }
    Column {
        property int childWidth: width - 2 * padding
        property int rows: 5
        id: rt02
        width: parent.width * 0.4
        height: parent.height
        anchors.right: parent.right
        //columns: 2
        //rows:3
        spacing: 6
        padding: 6
        signal accepted

        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("NVRam")
            onButtonClicked: {
                popupLoader.push("../pages/NVRamSettings.qml","show2")
                printSetConn.enabled = true
                //holdState()
            }
            Connections {
                id: printSetConn
                target: popupLoader.item
                ignoreUnknownSignals: true
                enabled: false
                onAccepted: {
                    recoverState()
                    printSetConn.enabled = false
                }
                onCanceled: {
                    recoverState()
                    printSetConn.enabled = false
                }
            }
        }
        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("NVRam2")
            onButtonClicked: {
                popupLoader.push("../pages/NVRamSets2.qml","show")
                nvs2.enabled = true
                //holdState()
            }
            Connections {
                id: nvs2
                target: popupLoader.item
                ignoreUnknownSignals: true
                enabled: false
                onAccepted: {
                    recoverState()
                   nvs2.enabled = false
                }
                onCanceled: {
                    recoverState()
                    nvs2.enabled = false
                }
            }
        }
        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("Scan")
            onButtonClicked: {
                popupLoader.push("../pages/ScanSettings.qml","show")
                timeSetConn.enabled = true
                //holdState()
            }
            Connections {
                id: timeSetConn
                target: popupLoader.item
                ignoreUnknownSignals: true
                enabled: false
                onAccepted: {
                    recoverState()
                    timeSetConn.enabled = false
                }
                onCanceled: {
                    recoverState()
                    timeSetConn.enabled = false
                }
            }
        }
        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("FrontPanel")
            onButtonClicked: {
               popupLoader.push("../pages/FrontPanelSet.qml","show")
                discom.enabled = true

                holdState()
            }
            Connections {
                id: discom
                enabled: false
                target: popupLoader.item
                ignoreUnknownSignals: true
                onAccepted: {
                    recoverState()
                    for (var i = 0; i < 4; i++) {
                        defaultInfoSet[i] = infoSet[i]
                    }
                    console.log(infoSet)
                    discom.enabled = false
                }
                onCanceled: {
                    recoverState()
                    discom.enabled = false
                }
            }
        }
        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("PrintManuf.Report")
            onButtonClicked: {
                popupLoader.push("../pages/PrintReport.qml","show")

                pascom.enabled = true
                //holdState()
            }
            Connections {
                id: pascom
                enabled: false
                target: popupLoader.item
                ignoreUnknownSignals: true
                onAccepted: {
                    recoverState()

                    console.log("pasxcom")
                    pascom.enabled = false
                }
                onCanceled: {
                    recoverState()
                    pascom.enabled = false
                }
            }
        }
    }
    Connections {
        id: acceptAction01
        target: acceptButton
        enabled: false
        onButtonClicked: {

            acceptButton.popRole()
            cancelButton.popRole()
            acceptAction01.enabled = false

            accepted()
        }
    }
    Component.onCompleted: {
        acceptAction01.enabled = true        
        genInfoList()
        acceptButton.pushRole([acceptAction01, qsTr("Accept"),"idle"])
        cancelButton.pushRole([null,null,"invisible"])
    }

}
