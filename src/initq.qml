import QtQuick 2.7
import com.kp.initc 1.0

Item {
    id: initq
    property alias state: loadingBar.state
    signal initAllSucceed

    Initc {
        id: initProc
    }

    Loader {
        //用states改写
        id: loadingBar
        property bool timeLimitExceeded: false
        property bool scanButtonPressed: false
        source: "InitBox.qml"
        state: "idle"

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
        onLoaded: {

            item.openPopup()
            loadingBar.state = "onSelfTest"
        }

        onStateChanged: {
            if (loadingBar.state === "onSelfTest") {

                item.labelText = qsTr(
                            "System is running a self-test.\nPlease do a X-ray scan after 30s.")

                initProc.operateSelfTest()
            } else if (loadingBar.state === "selfTestOK") {
                item.labelText = qsTr(
                            "Self-test succeed.\nWhen the Light is on, LONG PRESS the exposure button to conduct a base scan.")
                initProc.prepareFirstScan()
                 loadingBar.enabled = true
            } else if (loadingBar.state === "firstScanPrepared") {
                console.log("Light on.")

            } else if (loadingBar.state === "timeNotExceeded") {
                item.labelText = qsTr(
                            "firstscan interrupted, LONG PRESS the button again")
            } else if (loadingBar.state === "onScan") {
                item.labelText = qsTr(
                            "System is setting reference image and\ntesting system function. \nwhen finished the Main menu will show up.")
                initProc.operateFirstScan()
            } else if (loadingBar.state === "finished") {
                item.closePopup()
                loadingBar.source = ""
                initAllSucceed()
            }
        }

        anchors.fill: parent
        focus: true
        enabled: false
        Keys.enabled: true
        Keys.onPressed: {
            if (event.key === Qt.Key_A) {
                scanButtonPressed = true
                console.log("A pressed")
                if (loadingBar.state === "firstScanPrepared"
                        || loadingBar.state === "timeNotExceeded") {
                    console.log("timerstarted")
                    tim2.restart()
                    timeLimitExceeded = false
                }
            }
            event.accept
        }
        Keys.onReleased: {
            scanButtonPressed = false
            console.log("A released")
            if (event.key === Qt.Key_A) {
                if ((loadingBar.state === "firstScanPrepared"
                     || loadingBar.state === "timeNotExceeded")
                        && timeLimitExceeded === false) {
                    tim2.stop()
                    loadingBar.state = "timeNotExceeded"
                }
            }
            event.accept
        }
        function selfTestEnded(cd) {
            if (cd === 1) {
                loadingBar.state = "selfTestOK"
            } else {
                loadingBar.state = "ERROR"
                item.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }

        function firstScanReady(cd) {
            if (cd === 1) {
                loadingBar.state = "firstScanPrepared"
            } else {
                loadingBar.state = "ERROR"
                item.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }

        function firstScanEnded(cd) {
            if (cd === 1) {
                loadingBar.state = "finished"
            } else {
                loadingBar.state = "ERROR"
                item.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }
    }
    Timer {
        id: tim2
        interval: 10 ///设置按钮必须按压的时间ms,测试时在OS中取消键盘长按
        running: false
        repeat: false
        onTriggered: {
            if (loadingBar.scanButtonPressed === true) {
                loadingBar.timeLimitExceeded = true
                loadingBar.state = "onScan"
            }
        }
    }

    Connections {
        target: initProc
        onHandleSelfTestResults: {
            loadingBar.selfTestEnded(result)
        }
        onHandleFirstScanResults: {
            loadingBar.firstScanEnded(result)
        }
        onFirstScanReady: {
            loadingBar.firstScanReady(result)
        }
    }
}
