import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Page {
    title: "Animationtest"
    property int myItemHeight: 40
    property int aniDur: 50
    property var animated: [false, false, false, false, false,false]
    property var statusArr: [false, false, false, false, false]
    function changeEffAniDis(n) {
        for (var i = 0; i < 6; i++) {
            animated[i] = false
        }
        animated[n] = true
        update()
    }
    function update() {
        animated = animated
        statusArr = statusArr
    }
    Component {
        id: content0
        Rectangle {
            property alias inputText: input0.text
            height: 100
            anchors.top: parent.top

            TextField {
                id:input0
                text: ""
                focus: true
                onActiveFocusChanged: {
                    if(input0.activeFocus===true) input0.selectAll()
                }
                validator: IntValidator {
                    bottom: 0
                    top: 999
                }

            }
            Button {
                anchors.top: input0.bottom
                 id:continue0
                text: qsTr("Next")
                enabled: !input0.text===""
            }
        }
    }
    Component {
        id: content1
        Column {
            anchors.top:parent.top
            height: 100
        TextField {
            id:input1
            focus: true
            text: "body1"
            onFocusChanged: {
                if(input1.activeFocus===true) input1.selectAll()
            }

        }
        Button {
            id:continue1
            text: qsTr("continue")
        }
        }
    }
    Component {
        id: content2
        Column {
            anchors.top:parent.top
            height: 100

        TextField {
            id:input2
            focus: true
            text: "body1"
            onFocusChanged: {
                if(input2.activeFocus===true) input2.selectAll()
            }

        }
        Button {
            id:continue2
            text: qsTr("continue")
        }
      }
    }
    Component {
        id: content3
        Column {
            anchors.top:parent.top
            height: 100

        TextField {
            id:input3
            focus: true
            text: "body1"

        }
        Button {
            id:continue3
            text: qsTr("continue")
        }
      }
    }
    Component {
        id: content4
        Column {
            anchors.top:parent.top
            height: 100

        TextField {
            id:input4
            focus: true
            text: "body1"
        }
        Button {
            id:continue4
            text: qsTr("continue")
        }
      }
    }
    Component.onCompleted: {
        content0.createObject(ch0.contentParent)
        content1.createObject(ch1.contentParent)
        content2.createObject(ch2.contentParent)
        content3.createObject(ch3.contentParent)
        content4.createObject(ch4.contentParent)
        console.log(content0.parent)
        ch0.roundButton.toggle()
    }

    Rectangle {
        id: panel
        width: 200
        anchors.top: parent.top
        anchors.left:parent.left
        anchors.topMargin: 10
        color: "transparent"
        Behavior on y {
            NumberAnimation {
            }
        }
        ButtonGroup{
            id:buttobG
            buttons: [ch0.roundButton,ch1.roundButton,ch2.roundButton,ch3.roundButton,ch4.roundButton]
           onCheckedButtonChanged: {

           }
        }

        Channel {
            id: ch0
            anchors.top: parent.top
            height: myItemHeight
            width: parent.width
            buttonText: statusArr[0] ? "✔" : "1"
            labelText: qsTr("Operator")
            bannerHeight: myItemHeight
            visibleHeight: myItemHeight
            z: 0

            onRbCheckedChanged: {
                changeEffAniDis(1)
                if (checked === true) {

                    ch0.visibleHeight = myItemHeight + ch0.contentHeight
                } else {
                    ch0.visibleHeight = myItemHeight
                }
            }

        }

        Channel {
            id: ch1
            y: ch0.y + ch0.visibleHeight
            height: myItemHeight
            width: parent.width
            labelText: qsTr("Patient ID")
            buttonText: statusArr[1] ? "✔" : "2"
            bannerHeight: myItemHeight
            visibleHeight: myItemHeight
            z: 1

            Behavior on y {
                NumberAnimation {
                    duration: animated[1] ? aniDur : 0
                }
            }
            onRbCheckedChanged: {
                changeEffAniDis(2)
                if (checked === true) {

                    ch1.visibleHeight = myItemHeight + ch1.contentHeight
                } else {
                    ch1.visibleHeight = myItemHeight
                }
            }
        }
        Channel {
            id: ch2
            y: ch1.y + ch1.visibleHeight
            height: myItemHeight
            width: parent.width
            labelText: qsTr("Patient Age")
            buttonText: statusArr[2] ? "✔" : "3"
            bannerHeight: myItemHeight
            visibleHeight: myItemHeight

            Behavior on y {
                NumberAnimation {
                    duration: animated[2] ? aniDur : 0
                }
            }
            z: 2
            onRbCheckedChanged: {
                changeEffAniDis(3)
                if (checked === true) {

                    ch2.visibleHeight = myItemHeight + ch2.contentHeight
                } else {
                    ch2.visibleHeight = myItemHeight
                }
            }
        }
        Channel {
            id: ch3
            y: ch2.y + ch2.visibleHeight
            height: myItemHeight
            width: parent.width
            labelText: qsTr("Gender")
            buttonText: statusArr[3] ? "✔" : "4"
            bannerHeight: myItemHeight
            visibleHeight: myItemHeight

            Behavior on y {
                NumberAnimation {
                    duration: animated[3] ? aniDur : 0
                }
            }
            z: 2
            onRbCheckedChanged: {
                changeEffAniDis(4)
                if (checked === true) {

                    ch3.visibleHeight = myItemHeight + ch3.contentHeight
                } else {
                    ch3.visibleHeight = myItemHeight
                }
            }
        }
        Channel {
            id: ch4
            y: ch3.y + ch3.visibleHeight
            height: myItemHeight
            width: parent.width
            labelText: qsTr("Race")
            buttonText: statusArr[4] ? "✔" : "5"
            bannerHeight: myItemHeight
            visibleHeight: myItemHeight

            Behavior on y {
                NumberAnimation {
                    duration: animated[4] ? aniDur : 0
                }
            }
            z: 3
            onRbCheckedChanged: {
                changeEffAniDis(4)
                if (checked === true) {

                    ch4.visibleHeight = myItemHeight + ch4.contentHeight
                } else {
                    ch4.visibleHeight = myItemHeight
                }
            }
        }
        Rectangle{
            id:plate
            y: ch4.y + ch4.visibleHeight
            width: parent.width
            height: 100
            //color: "#ffffff"
            z:4
            Behavior on y {
                NumberAnimation {
                    duration: animated[5] ? aniDur : 0
                }
            }
        }
    }
}
