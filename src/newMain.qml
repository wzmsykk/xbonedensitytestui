import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0
import com.kp.settings 1.0
import "./components"
ApplicationWindow {
    id: root
    width: 800
    height: 500
    visible: true
    title: qsTr("Test App")
    property bool testui: false
    property int paneHeight: 200
    property int paneWidth: 300
    property int vMargin: 10
    property int hMargin: 10
    property int toolBarHeight: 0.14 * root.height
    property string style:"simple"
    property string lang: "en"
    property string primaryColor:style==="simple"?"#000000":"#ffffff"
    property var infoSet: [-1,-1,0,0]
    property var currInfo:[]
    property var defaultInfoSet: [-1,-1,0,0]
    property var printSet: [1,0,0]
    property int currentFingerSelection: 7
    property bool resultReady: false
    property int hiddenSetCount: 0
    function showHiddenSettings(){
        console.log("showed")
        popupLoader.push("../pages/HiddenSettings.qml")
        hideAction.enabled=false

    }

    AnchorScript {
        id: anchorScript
    }

    background: Rectangle {
        id: backRec
        anchors.fill: parent
        Image {
            id: backGImage
            source: "pic/timg.jpg"
            anchors.fill: parent
            //sourceSize: Qt.size(parent.width,parent.height)
            fillMode: Image.Stretch
            smooth: true
            visible: false
        }

        GaussianBlur {
            id: backGBlur
            anchors.fill: backGImage
            source: backGImage
            radius: 64
            samples: 32
            visible: style==="simple"?false:true
        }
    }
    Loader {
        id: ld
        z:2
        source: testui ? "" : "initq.qml"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.bottom: toolbar.top
        Component.onCompleted: {

        }

        Connections {
            target: ld.item
            onInitAllSucceed: {
                ld.enabled = false
                ld.visible = false
                anchorScript.state="normal"
            }
        }
    }
    PopupLoader {
        id: popupLoader
        anchors.bottom: toolbar.top
        anchors.left: undefined
        anchors.right: parent.right
        anchors.top: parent.top

        z: 6
    }
    Loader{
        id:hiddenLoader
        anchors.bottom: toolbar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        source: ""
        z:5
        Connections{
            target: hiddenLoader.item
            onAccepted:{
                hiddenLoader.source=""
            }
        }
    }

    Item {
        id: centPoint//Anchor Positioner Central Point
        x: parent.width / 2
        y: (parent.height - toolbar.height - vMargin) / 2
    }
    InputPanel {
        id: inputPanel
        anchors.bottom: toolbar.top
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: undefined
        backgI: backGImage
        z:7
    }

    Rectangle {
        id: toolbar
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: vMargin
        anchors.left: parent.left
        anchors.leftMargin: hMargin
        anchors.right: parent.right
        anchors.rightMargin: hMargin
        height: toolBarHeight

        Label {
            id: timeText
            text: new Date().toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
            anchors.top: parent.top
            anchors.left: parent.left
            height: parent.height
            color: primaryColor
            opacity: 0.7
            fontSizeMode: Text.VerticalFit
            minimumPixelSize: 10
            font.pixelSize: 72
            MouseArea{
                id:hideAction
                anchors.fill: parent
                onClicked: {
                    if(hiddenSetTimer.running===false) hiddenSetTimer.restart();
                    hiddenSetCount++;
                    if(hiddenSetCount>=5){
                        hiddenSetTimer.stop()
                        hiddenSetCount=0
                        showHiddenSettings()
                    }
                }

            }
        }
        Timer{
            id:hiddenSetTimer
            interval: 1000
            repeat: false
            running: false
            triggeredOnStart: false
            onTriggered: {
                hiddenSetCount=0
            }
        }

        CancelButton {
            id: thirdButton
            anchors.bottom: parent.bottom
            anchors.margins: 8
            anchors.right: cancelButton.left
            anchors.rightMargin: vMargin
            anchors.top: parent.top
            height: parent.height * 0.85
            width: parent.width * 0.22
            state: "idle"
        }
        CancelButton {
            id: cancelButton
            anchors.bottom: parent.bottom
            anchors.margins: 8
            anchors.right: acceptButton.left
            anchors.rightMargin: vMargin
            anchors.top: parent.top
            height: parent.height * 0.85
            width: parent.width * 0.22
        }

        AcceptButton {
            id: acceptButton
            anchors.bottom: parent.bottom
            anchors.margins: 8
            anchors.right: parent.right
            anchors.top: parent.top
            width: parent.width * 0.22
            height: parent.height * 0.85
        }
    }

    FuzzyPanel {
        id: lefttop
        anchors.left: parent.left
        anchors.bottom: centPoint.top
        anchors.right: centPoint.left
        anchors.top: parent.top
        anchors.leftMargin: hMargin
        anchors.topMargin: vMargin
        anchors.bottomMargin: hMargin / 2
        anchors.rightMargin: vMargin / 2
        state: "normal"
        titleHeight: 45
        titleColor: "#03A9F4"
        titleText.text: qsTr("Start")
        target: backGImage
        Rectangle {
            id: lefttopContentArea
            anchors.top: lefttop.titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 1
            color: "transparent"
            state: "01"
            states: [
                State {
                    name: "01"
                    PropertyChanges{
                        target: lt02
                        opacity:0
                        visible:false
                        source:""

                    }
                    StateChangeScript{
                        script: lt02a.enabled=false
                    }

                },
                State {
                    name: "02"
                    PropertyChanges{
                        target: lt02
                        opacity:1
                        visible:true
                    }

                    PropertyChanges {
                        target: lt01
                        opacity: 0
                    }
                    PropertyChanges{
                        target: lt02
                        source:"pages/PatientSet.qml"
                    }
                    StateChangeScript{

                        script:{ lt02a.enabled=true
                            currInfo=defaultInfoSet}
                    }
                }
            ]

            Row {
                id: lt01
                height: parent.height * 0.7
                width: parent.width * 0.9
                anchors.centerIn: parent
                spacing: 20
                Image {
                    id: img00
                    source: "icons/00.svg"
                    height: parent.height * 0.8
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height * 0.8
                }
                Text {
                    color: primaryColor
                    text: qsTr("Start")
                    anchors.verticalCenter: parent.verticalCenter
                    fontSizeMode: Text.Fit
                    font.pixelSize: 72
                }
            }
         Loader{
             id:lt02
             anchors.fill: parent
             opacity: 0
             source: ""
             Connections{
                 id:lt02a
                 target: lt02.item
                 enabled:false
                 onAccepted:{

                     popupLoader.push("../pages/WorkPage.qml")
                     popupLoader.state="show"
                     inputPanel.state="hide"
                     waitScanResult.enabled=true
                 }
                 onCanceled:{
                     anchorScript.state="normal"
                 }
             }

             Connections{
                 id:waitScanResult
                 enabled:false
                 target: popupLoader.item
                 ignoreUnknownSignals: true
                 onCanceled:{
                      popupLoader.clear()
                     lt02.item.recoverStates()
                     waitScanResult.enabled=false
                 }
                 onAccepted:{
                     waitScanResult.enabled=false
                     popupLoader.clear()
                     anchorScript.state="normal"
                     anchorScript.state="leftbottomshow"


                 }
             }

         }

        }
    }
    FuzzyPanel {
        id: leftbottom
        anchors.bottom: toolbar.top
        anchors.left: parent.left
        anchors.top: centPoint.top
        anchors.right: centPoint.left
        anchors.leftMargin: hMargin
        anchors.topMargin: vMargin / 2
        anchors.bottomMargin: vMargin
        anchors.rightMargin: hMargin / 2
        titleHeight: 45
        titleColor: "#03A9F4"
        titleText.text: qsTr("Settings")
        target: backGImage
        Rectangle {
            id: leftbottomContentArea
            anchors.top: leftbottom.titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 1
            color: "transparent"
            state: "01"
            states: [
                State {
                    name: "01"
                    PropertyChanges {
                        target: lb02
                        visible: false
                        opacity:0
                        source:""
                    }
                },
                State {
                    name: "02"
                    PropertyChanges {
                        target: lb02
                        visible: true
                        opacity:1
                        source:"pages/ResultPage.qml"
                    }
                    PropertyChanges {
                        target: lb01
                        opacity: 0

                    }
                    StateChangeScript{
                        script: {
                            currInfo=defaultInfoSet
                        }
                    }
                }
            ]

            Row {
                id: lb01
                height: parent.height * 0.7
                width: parent.width * 0.9
                anchors.centerIn: parent
                spacing: 6
                Image {
                    id: img01

                    source: "icons/01.svg"
                    height: parent.height * 0.8
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height * 0.8
                }
                Text {
                    text: qsTr("Results")
                    width: parent.width - img00.width
                    anchors.verticalCenter: parent.verticalCenter

                    fontSizeMode: Text.Fit
                    font.pixelSize: 72
                    color: primaryColor
                }
            }

            Loader {
                id: lb02
                visible: false
                anchors.fill: parent
            }
        }
    }
    FuzzyPanel {
        id: righttop
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: centPoint.top
        anchors.left: centPoint.left
        anchors.leftMargin: hMargin / 2
        anchors.topMargin: vMargin
        anchors.bottomMargin: vMargin / 2
        anchors.rightMargin: hMargin
        titleText.text: qsTr("Results")
        titleHeight: 45
        titleColor: "#03A9F4"
        target: backGImage

        Rectangle {
            id: righttopContentArea
            anchors.top: righttop.titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 1
            color: "transparent"
        state: "01"
        states: [
            State {
                name: "01"
                PropertyChanges {
                    target: rt02
                    visible: false
                    source:""
                    opacity:0
                }
            },
            State {
                name: "02"
                PropertyChanges {
                    target: rt02
                    visible: true
                    source:"pages/SettingPage.qml"
                    opacity:1
                }
                PropertyChanges {
                    target: rt01
                    opacity: 0
                }
            }
        ]

        Row {
            id: rt01
            height: parent.height * 0.7
            width: parent.width * 0.9
            anchors.centerIn: parent
            spacing: 20
            Image {
                id: img10
                source: "icons/02.svg"
                height: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                width: parent.height * 0.8
            }
            Text {
                color: primaryColor
                text: qsTr("Settings")
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-img10.width
                fontSizeMode: Text.Fit
                font.pixelSize: 72
            }
        }
        Loader {
            id: rt02
            anchors.fill: parent
            visible: false
            opacity: 0
            source: ""
            Behavior on opacity {NumberAnimation{}}
            Connections{
                target: rt02.item
                ignoreUnknownSignals: true
                onAccepted:{
                    anchorScript.state="normal"
                }
            }
        }
        }
    }
    FuzzyPanel {
        id: rightbottom
        anchors.right: parent.right
        anchors.top: centPoint.top
        anchors.left: centPoint.left
        anchors.bottom: toolbar.top
        anchors.leftMargin: hMargin / 2
        anchors.topMargin: vMargin / 2
        anchors.bottomMargin: vMargin
        anchors.rightMargin: hMargin
        titleText.text: qsTr("About")
        titleHeight: 45
        titleColor: "#03A9F4"
        target: backGImage
        Rectangle {
            id: rightbottomContentArea
            anchors.top: rightbottom.titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 1
            color: "transparent"
        state: "01"
        states: [
            State {
                name: "01"
            },
            State {
                name: "02"
                PropertyChanges {
                    target: rb02
                    visible: true
                }
                PropertyChanges {
                    target: rb01
                    opacity: 0
                }

            }
        ]

        Row {
            id: rb01
            height: parent.height * 0.7
            width: parent.width * 0.9
            anchors.centerIn: parent
            spacing: 20
            Image {
                id: img11
                source: "icons/03.svg"
                height: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                width: parent.height * 0.8
            }
            Text {
                color: primaryColor
                text: qsTr("About")
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-img10.width
                fontSizeMode: Text.Fit
                font.pixelSize: 72
            }
        }
        Grid{
            id:rb02
            visible: false
            Text {
                text: qsTr("About Text Here")
            }
        }
        }
    }
}
