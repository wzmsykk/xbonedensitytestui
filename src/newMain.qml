import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

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
    property string primaryColor: "#607D8B"


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
        state: "hide"
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
                text: new Date().toLocaleTimeString(Qt.locale(),
                                                    Locale.ShortFormat)
                anchors.top:parent.top
                anchors.left: parent.left
                height: parent.height
                color: "white"
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
                height: parent.height*0.85
                width: parent.width*0.22
            }

            AcceptButton {
                id: acceptButton
                anchors.bottom: parent.bottom
                anchors.margins: 8
                anchors.right: parent.right
                anchors.top: parent.top
                width: parent.width*0.22
                height: parent.height*0.85
            }

    }
    FuzzyPanel{
        id:workPage
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.left
        target: backGImage
        state: "hide"
        states: [State {
                name: "hide"
                AnchorChanges {
                    target:workPage
                    anchors.top:parent.top
                    anchors.left: parent.right
                    anchors.bottom: toolbar.top
                    anchors.right: undefined

                }
                PropertyChanges{
                    target: workPage
                    width:parent.width
                    visible:false
                }
            },State {
                name: "show"
                PropertyChanges {
                    target: workPage
                    width:undefined
                    visible:true
                }
                AnchorChanges {
                    target:workPage
                    anchors.top:parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: toolbar.top

                }
            }]
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
        Rectangle{
            id:lefttopContentArea
            anchors.top: lefttop.titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 1
            color: "transparent"
            state: "01"
            states: [State {
                    name: "01"

                },State {
                    name: "02"
                    PropertyChanges {
                        target: content02
                        visible:true
                    }
                    PropertyChanges{
                        target: content01
                        opacity:0
                    }                }]
            Behavior on opacity {NumberAnimation{}}
            Row{
                id:content01
                height:parent.height*0.7
                width: parent.width*0.9
                anchors.centerIn:parent
                spacing: 20
                Image {
                    id: img01
                    source: "file:"+applicationDirPath+"/00.svg"
                    height: parent.height*0.8
                    width: parent.height*0.8
                }
                Text{
                    text:"Start"
                    fontSizeMode:Text.Fit
                    font.pixelSize: 72
                    color: primaryColor
                }
            }

            Grid{
                id:content02
                columns: 3
                visible: false
                FlatTextField {
                    id: text1
                    enabled: true
                    focus: true
                    height: 80
                    width: 200
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
        states: [
            State {
                name: "expand"

            },
            State {
                name: "normal"
                AnchorChanges {
                    target: leftbottom.titleText
                    anchors.horizontalCenter: undefined
                    anchors.left: parent.left
                }
                AnchorChanges {
                    target: leftbottom
                    anchors.bottom: toolbar.top
                    anchors.left: parent.left
                    anchors.top: centPoint.top
                    anchors.right: centPoint.left
                }
            }
        ]
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
        states: [
            State {
                name: "expand"

            },
            State {
                name: "normal"
                AnchorChanges {
                    target: righttop.titleText
                    anchors.horizontalCenter: undefined
                    anchors.left: parent.left
                }
                AnchorChanges {
                    target: righttop
                    anchors.bottom: centPoint.top
                    anchors.left: centPoint.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                }
            }
        ]
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
        states: [
            State {
                name: "expand"

            },
            State {
                name: "normal"
                AnchorChanges {
                    target: rightbottom.titleText
                    anchors.horizontalCenter: undefined
                    anchors.left: parent.left
                }
                AnchorChanges {
                    target: rightbottom
                    anchors.bottom: toolbar.top

                    anchors.right: parent.right
                    anchors.top: centPoint.top
                    anchors.left: centPoint.left
                }
            }
        ]
    }

}
