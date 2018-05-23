import QtQuick 2.7
import com.kp.initc 1.0
import "./components"
Item {
    id: initq
    anchors.fill: parent
    signal initAllSucceed
    Initc {
        id: initProc
    }



    InitBox {
        //用states改写
        id: loadBox
        property bool timeLimitExceeded: false
        property bool scanButtonPressed: false
        property int demandKey: Qt.Key_A
        state:  "onSelfTest"
        anchors.fill: parent
        focus: true
        enabled: true
        Keys.enabled: true
        states: [
            State {
                name: "idle"
            },
            State {
                name: "onSelfTest"
            },
            State {
                name: "selfTestOK"
            },
            State {
                name: "firstScanPrepared"
            },
            State {
                name: "timeNotExceeded"
            },
            State {
                name: "onScan"
            },
            State {
                name: "finished"
            },
            State {
                name: "ERROR"
            }
        ]


        onStateChanged: {
            if (loadBox.state === "onSelfTest") {
                loadBox.openPopup()

                loadBox.labelText = qsTr(
                            "System is running a self-test.\nPlease do a X-ray scan after 30s.")

                initProc.operateSelfTest()
            } else if (loadBox.state === "selfTestOK") {
                loadBox.labelText = qsTr(
                            "Self-test succeed.\nWhen the Light is on, LONG PRESS the exposure button to conduct a base scan.")
                initProc.prepareFirstScan()
                showTimeSetting()
                 loadBox.enabled = true

            } else if (loadBox.state === "firstScanPrepared") {
                console.log("Light on.")
                acceptButton.state="idle"

            } else if (loadBox.state === "timeNotExceeded") {
                loadBox.labelText = qsTr(
                            "firstscan interrupted, LONG PRESS the button again")
            } else if (loadBox.state === "onScan") {
                loadBox.labelText = qsTr(
                            "System is setting reference image and\ntesting system function. \nwhen finished the Main menu will show up.")
                initProc.operateFirstScan()
            } else if (loadBox.state === "finished") {
                loadBox.closePopup()
                 acceptButton.popRole()
                acceptButton.state="invisible"
                initAllSucceed()
            }
        }


        Keys.onPressed: {
            if (event.key === demandKey) {
                scanButtonPressed = true
                console.log("A pressed")
                if (loadBox.state === "firstScanPrepared"
                        || loadBox.state === "timeNotExceeded") {
                    console.log("timerstarted")
                    tim2.restart()
                    timeLimitExceeded = false
                }
            }
        }
        Keys.onReleased: {
            scanButtonPressed = false
            console.log("A released")
            if (event.key === demandKey) {
                if ((loadBox.state === "firstScanPrepared"
                     || loadBox.state === "timeNotExceeded")
                        && timeLimitExceeded === false) {
                    tim2.stop()
                    loadBox.state = "timeNotExceeded"
                }
            }
        }
        function selfTestEnded(cd) {
            if (cd === 1) {
                loadBox.state = "selfTestOK"
            } else {
                loadBox.state = "ERROR"
                loadBox.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }

        function firstScanReady(cd) {
            if (cd === 1) {
                loadBox.state = "firstScanPrepared"
            } else {
                loadBox.state = "ERROR"
                loadBox.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }

        function firstScanEnded(cd) {
            if (cd === 1) {
                loadBox.state = "finished"
            } else {
                loadBox.state = "ERROR"
                loadBox.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }
        function showTimeSetting(){
            popupLoader.push("../pages/TimeSettings.qml","show2")

            timeconn.target=popupLoader.item
            timeconn.enabled=true
        }
            Connections{
                id:timeconn
                target: popupLoader.item
                ignoreUnknownSignals: true
                enabled:false
                onAccepted:{
                    popupLoader.pop()
                    inputPanel.state="hide"
                    timeconn.enabled=false
                }
                onCanceled:{
                    popupLoader.pop()
                    inputPanel.state="hide"
                     timeconn.enabled=false
                }
            }
    }
    Timer {
        id: tim2
        interval: 10 ///设置按钮必须按压的时间ms,测试时在OS中取消键盘长按
        running: false
        repeat: false
        onTriggered: {
            if (loadBox.scanButtonPressed === true) {
                loadBox.timeLimitExceeded = true
                loadBox.state = "onScan"
            }
        }
    }

    Connections {
        target: initProc
        onHandleSelfTestResults: {
            loadBox.selfTestEnded(result)
        }
        onHandleFirstScanResults: {
            loadBox.firstScanEnded(result)
        }
        onFirstScanReady: {
            loadBox.firstScanReady(result)
        }
    }

    Connections{
        id:acb
        target:acceptButton
        enabled:true
        onButtonClicked:{
            if(loadBox.state === "firstScanPrepared"
                    || loadBox.state === "timeNotExceeded"){
            loadBox.state="onScan"
            acceptButton.state="invalid"
            }
        }
    }
    Component.onCompleted:{
        loadBox.forceActiveFocus()
        acceptButton.state="idle"
        acceptButton.state="invalid"
        acceptButton.pushRole([acb,qsTr("Scan")])
    }
    Component.onDestruction: {
        acceptButton.popRole()
    }
}



