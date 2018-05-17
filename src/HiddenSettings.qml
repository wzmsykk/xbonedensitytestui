import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.settings 1.0



FuzzyPanel {
    anchors.fill: parent
    implicitHeight: 480
    implicitWidth: 600

    target: backGImage
    ListModel{
        id:myl
    }

    function genInfoList(){
        myl.clear();
        myl.append({"name":qsTr("ByteUsed"),"num":mySettings.byteUsed.toString()})
        myl.append({"name":qsTr("CheckSum"),"num":mySettings.checkSum.toString()})
        myl.append({"name":qsTr("PMTValue"),"num":mySettings.PMTValue.toString()})
        myl.append({"name":qsTr("T-offset"),"num":mySettings.toffset.toString()})
        myl.append({"name":qsTr("Z-offset"),"num":mySettings.zoffset.toString()})
        myl.append({"name":qsTr("Gain"),"num":mySettings.gain.toString()})
        myl.append({"name":qsTr("Offset"),"num":mySettings.offset.toString()})
        myl.append({"name":qsTr("AvgMu"),"num":mySettings.avgmu.toString()})
        myl.append({"name":qsTr("SerialNum."),"num":mySettings.serial.toString()})
        myl.append({"name":qsTr("S/W Ver."),"num":mySettings.sver.toString()})
        myl.append({"name":qsTr("DB Ver."),"num":mySettings.dver.toString()})
        myl.append({"name":qsTr("BMD #1"),"num":mySettings.bmd1.toString()})
        myl.append({"name":qsTr("BMD #2"),"num":mySettings.bmd2.toString()})
        myl.append({"name":qsTr("BMD #3"),"num":mySettings.bmd3.toString()})
        myl.append({"name":qsTr("SNR"),"num":mySettings.snr.toString()})
        myl.append({"name":qsTr("Mu"),"num":mySettings.mu.toString()})

    }

    Grid {
        id:gr1
        width: parent.width*0.6
        height: parent.height
        spacing: 6
        columns: 2
        rows:8
        flow: Grid.TopToBottom
        property var customData:
            ListModel{
            ListElement{
                name:qsTr("Byte")
            }
        }

        property int childWidth:(width-(spacing+2)*columns)/columns
        property int childHeight: (height-(spacing+2)*rows)/rows
        Repeater {
            model: myl
            Text {
                id: devText
                width: gr1.childWidth
                height: gr1.childHeight
                text: name+":"+num
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
        function recoverState() {
            acceptAction01.enabled = true
            rt02.visible = true
            rt02.enabled = true
            cancelButton.state = "invisible"
            popupLoader.source = ""
            popupLoader.state = "hide"
            inputPanel.state = "hide"
        }
        function holdState() {
            acceptAction01.enabled = false

            rt02.visible = false
            rt02.enabled = false
        }

        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("Print Settings")
            onButtonClicked: {
                popupLoader.source = "PrintSettings.qml"
                popupLoader.state = "show"
                printSetConn.enabled = true
                holdState()
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
            title: qsTr("Time Settings")
            onButtonClicked: {
                popupLoader.source = "TimeSettings.qml"
                popupLoader.state = "show2"
                inputPanel.state = "show"
                timeSetConn.enabled = true
                holdState()
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
            title: qsTr("Default Info Settings")
            onButtonClicked: {
                popupLoader.source = "PatientSet.qml"
                popupLoader.state = "show2"
                inputPanel.state = "show"
                discom.enabled = true
                cancelButton.state = "idle"
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
            title: qsTr("Password Settings")
            onButtonClicked: {
                popupLoader.source = "PasswordSettings.qml"
                popupLoader.state = "show2"
                inputPanel.state = "show"
                pascom.enabled = true
                cancelButton.state = "idle"
                holdState()
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
        FlatButton {
            width: parent.childWidth
            height: (parent.height - 5 * rt02.spacing) / rt02.rows
            title: qsTr("Other Settings")
            onButtonClicked: {
                popupLoader.source = "OtherSettings.qml"
                popupLoader.state = "show2"
                inputPanel.state = "show"
                pascom.enabled = true
                cancelButton.state = "idle"
                holdState()
            }
            Connections {
                id: othcom
                enabled: false
                target: popupLoader.item
                ignoreUnknownSignals: true
                onAccepted: {
                    recoverState()

                    console.log("othxcom")
                    othcom.enabled = false
                }
                onCanceled: {
                    recoverState()
                    othcom.enabled = false
                }
            }
        }


    }
    Connections {
        id: acceptAction01
        target: acceptButton
        enabled: false
        onButtonClicked: {
            //accepted()
            acceptButton.popRole();
            acceptAction01.enabled = false
        }
    }
    Component.onCompleted: {
        acceptAction01.enabled = true
        genInfoList()
        acceptButton.pushRole([acceptAction01,qsTr("Accept")])
    }
}
