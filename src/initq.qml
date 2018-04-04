import QtQuick 2.7
import com.kp.initc 1.0

Item {
    id: initq
    signal initAllSucceed
    Initc {
        id: initProc
    }

    Loader {
        id: loadingBar
        property bool scanButtonPressed: false
        property bool prepareScan: false
        property bool doingScan: false
        property int count: 0
        anchors.fill: parent
        Keys.enabled: false
        source: "InitBox.qml"
        focus: true
        onLoaded: {
            startLabel()
            item.openPopup()
            tim1.restart()
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_A) {
                count++
                console.log("key_a pressed", count)
                scanButtonPressed = true
                if (prepareScan == false &&doingScan==false) {

                    initProc.prepareFirstScan() //dont do much things here
                    prepareScan = true
                    tim2.restart()
                }
            }
            event.accept
        }
        Keys.onReleased: {
            if (event.key === Qt.Key_A) {
                console.log("key_a released")
                scanButtonPressed = false
                if (prepareScan === true)
                    firstscanInterrupted()
            }
            event.accept
        }
        function startLabel() {
            item.labelText = qsTr(
                        "System is running a self-test.\nPlease do a X-ray scan after 30s.")
        }
        function nextStep(cd) {
            if (cd === 1) {
                item.labelText = qsTr(
                            "Self-test succeed.\nPress the exposure button to conduct a base scan.")
                console.log(item.labelText)
                Keys.enabled = true
            } else {
                item.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }
        function scanLabel() {
            item.labelText = qsTr(
                        "System is setting reference image and\n"
                        + "testing system function. \nwhen finished the Main menu will show up.")
        }

        function firstscanInterrupted() {
            item.labelText = qsTr(
                        "firstscan interrupted, press the button again")
            tim2.stop()
            prepareScan = false
            initProc.firstScanClear()
        }
        function finalStep(cd) {
            if (cd === 1) {
                item.closePopup()
                focus = false
                initq.initAllSucceed()
            } else {
                item.labelText = qsTr("Failure.Error Code%1".arg(cd))
            }
        }
    }

    Timer {
        id: tim1
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            initProc.operateSelfTest()
        }
    }
    Timer {
        id: tim2
        interval: 10 ///设置按钮必须按压的时间ms,测试时在OS中取消键盘长按
        running: false
        repeat: false
        onTriggered: {
            if (loadingBar.scanButtonPressed == true) {
                initProc.operateFirstScan()
                loadingBar.scanLabel()
                loadingBar.prepareScan = false
                loadingBar.doingScan=true
            }
        }
    }

    Connections {
        target: initProc
        onHandleSTResults: {
            loadingBar.nextStep(result)
        }
        onHandleFSResults: {
            loadingBar.finalStep(result)
        }
    }
}
