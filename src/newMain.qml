import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0
ApplicationWindow {
    id: root
    width: 800
    height: 500
    visible: true
    title: qsTr("Test App")
    property bool testui: true
    property int paneHeight: 200
    property int paneWidth: 300
    property int vMargin: 10
    property int hMargin: 10
    property int toolBarHeight: 0.14 * root.height
    property string style:"simplex"
    property string primaryColor:style==="simple"?"#000000":"#ffffff"
    property var infoSet: []



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
        source: testui ? "" : "initq.qml"
        anchors.fill: parent
        Component.onCompleted: {

        }

        Connections {
            target: ld.item
            onInitAllSucceed: {
                ld.enabled = false
                ld.visible = false
                showui()
            }
        }
    }
    Loader{
        id:modalPopupLoader
        width: parent.width-2*hMargin
        anchors.top: parent.top
        anchors.bottom: toolbar.top
        anchors.right: parent.right
        anchors.left: undefined
        anchors.leftMargin: hMargin
        anchors.topMargin: vMargin
        anchors.bottomMargin: hMargin
        anchors.rightMargin: vMargin
        source: ""
        z:6
        transitions: Transition {
           AnchorAnimation{
               duration: 200
               easing.type: Easing.InOutQuad
           }
        }
        Behavior on opacity {
            NumberAnimation{}
        }

        state:"hide"
        states: [State {
            name: "hide"
            PropertyChanges{
                target: modalPopupLoader
                opacity:0
            }
            AnchorChanges {
                target: modalPopupLoader
                anchors.left:undefined
                anchors.right:  parent.right
            }
        },State {
            name: "show"
            AnchorChanges {
                target: modalPopupLoader
                anchors.left: parent.left
                anchors.right: undefined
            }
            PropertyChanges{
                target: modalPopupLoader
                opacity:1
            }
        }]
        Connections{
            target: modalPopupLoader.item
            onCanceled:{
                modalPopupLoader.state="hide"
                modalPopupLoader.source=""
            }
            onAccepted:{
                 modalPopupLoader.state="hide"
                anchorScript.state="leftbottomshow"
                modalPopupLoader.source=""
            }
        }

    }
    Loader{
        id:pageContent

        anchors.top: parent.top
        anchors.bottom: toolbar.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: hMargin
        anchors.topMargin: vMargin
        anchors.bottomMargin: hMargin
        anchors.rightMargin: vMargin

        source: ""

        Behavior on opacity {
            NumberAnimation{}
        }
        z:1

        Connections{
            target: pageContent.item
            onAccepted:{
                modalPopupLoader.source="WorkPage.qml"
                modalPopupLoader.state="show"
            }
            onCanceled:{
                pageContent.state="hide"
                pageContent.setSource("")
            }
        }

    }

    Item {
        id: centPoint
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
                        source: ""
                    }

                },
                State {
                    name: "02"
                    PropertyChanges{
                        target: lt02
                        opacity:1
                        source: "PatientSet.qml"
                    }

                    PropertyChanges {
                        target: lt01
                        opacity: 0
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
                    text: "Start"
                    anchors.verticalCenter: parent.verticalCenter
                    fontSizeMode: Text.Fit
                    font.pixelSize: 72
                }
            }
         Loader{
             id:lt02
             anchors.fill: parent
             opacity: 0
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
                },
                State {
                    name: "02"
                    PropertyChanges {
                        target: lb02
                        visible: true
                    }
                    PropertyChanges {
                        target: lb01
                        opacity: 0
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
                    text: "Results"
                    width: parent.width - img00.width
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Tahoma"
                    fontSizeMode: Text.Fit
                    font.pixelSize: 72
                    color: primaryColor
                }
            }

            Grid {
                id: lb02
                columns: 3
                visible: false
                anchors.fill: parent
                spacing: 6
                padding: 6
                Image {
                    id: boneImage
                    source: "/pic/tmp.jpg"
                    width: parent.width / 2-6
                    height: parent.height-12
                }
                Label{
                    text: qsTr("Result here")
                }
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
            },
            State {
                name: "02"
                PropertyChanges {
                    target: rt02
                    visible: true
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
                text: "Settings"
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width-img10.width
                fontSizeMode: Text.Fit
                font.pixelSize: 72
            }
        }
        Grid{

            visible: false
            id:rt02
            anchors.fill: parent
            columns: 2
            rows:3
            spacing: 6
            padding: 6
            /*FlatSpinBox {
                id:sp
                model: [1,2,3,4,5,6,7,8,9,10]
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Print Copied")
            }
            FlatSpinBox{
                id:sp2
                model: ["colored","grey"]
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Print Color")
            }*/
            FlatButton {
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Print Settings")
            }
            FlatButton {
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Time Settings")
            }
            FlatButton{
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Default Info Settings")
            }
            FlatButton{
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Password Settings")
            }
            FlatButton{
                width: (parent.width-3*rt02.spacing)/rt02.columns
                height: (parent.height-5*rt02.spacing)/rt02.rows
                title: qsTr("Other Settings")
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
                text: "About"
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
