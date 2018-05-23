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
                       name: qsTr("ExpCount"),
                       num: mySet.expcount.toString()
                   })
        myl.append({
                       name: qsTr("Accmu"),
                       num: mySet.accmu.toString()
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
            title: qsTr("CorruptNVRAM")
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
            title: qsTr("Reset NVRAM")
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
            title: qsTr("Clear Drift Mu Counter")
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
