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
    function nextstage(){
        stackView.push("ScanProgress.qml")
    }

    function update() {
        animated = animated
        statusArr = statusArr
    }
    Component {
        id: content0
        //Op ID
            Rectangle {
              id:body0
            height: 100
            anchors.top: parent.top

            TextField {
                id:input0
                text: "22"
                focus: true
                onActiveFocusChanged: {
                    if(input0.activeFocus===true) input0.selectAll()
                }
                validator: IntValidator {
                    bottom: 0
                    top: 999
                }
                Keys.onReturnPressed: {
                    open1.trigger()
                }

            }
            Action{
                id:open1
                onTriggered: {
                statusArr[0]=true
                ch1.click()}
            }

            Button {
                anchors.top: input0.bottom
                id:continue0
                text: qsTr("Next")
                action: open1

                enabled: !(input0.text==="")
            }
        }

    }
    Component {
        id: content1
        //Patient ID
        Column {
            anchors.top:parent.top
            height: 100
        TextField {
            id:input1
            focus: true
            text: ""
            validator: IntValidator {
                bottom: 0
                top: 9999
            }
            Keys.onReturnPressed: {
                open2.trigger()
            }
            onFocusChanged: {
                if(input1.activeFocus===true) input1.selectAll()
            }

        }
        Action{
            id:open2
            onTriggered: {
            statusArr[1]=true
            ch2.click()}
        }

        Button {
            id:continue1
            text: qsTr("Next")
            action: open2

            enabled: !(input1.text==="")
        }
        }
    }
    Component {
        id: content2
        //patient Age
        Column {
            anchors.top:parent.top
            height: 100

        TextField {
            id:input2
            focus: true
            text: ""
            validator: IntValidator {
                bottom: 0
                top: 200
            }
            onFocusChanged: {
                if(input2.activeFocus===true) input2.selectAll()
            }
            Keys.onReturnPressed: {
                open3.trigger()
            }

        }
        Action{
            id:open3
            onTriggered: {
            statusArr[2]=true
            ch3.click()}
        }
        Button {
            id:continue2
             enabled: !(input2.text==="")
             action: open3
            text: qsTr("Next")
        }
      }
    }
    Component {
        id: content3
        //gender
        Column {
            anchors.top:parent.top
            height: 100

        ComboBox{
                id:cb3
                model: [qsTr("Male"),qsTr("Female")]
        }
        Action{
            id:open4
            onTriggered: {
            statusArr[3]=true
            ch4.click()}
        }
        Button {
            id:continue3
            text: qsTr("continue")
            action: open4
        }
      }
    }
    Component {
        id: content4
        Column {
            anchors.top:parent.top
            height: 100

            ComboBox{
                    id:cb4
                    model: [qsTr("Caucasian"),qsTr("Asian"),qsTr("Others")]
            }
            Action{
                id:open5
                onTriggered: {
                statusArr[4]=true
                var flag=true
                    for(var i=0;i<5;i++)
                {
                    if(statusArr[i]===false)
                        flag=false
                }
                    if(flag===true) {
                    nextstage()
                    }

                }
            }
        Button {
            id:continue4
            text: qsTr("Next")
            action: open5

        }
      }
    }
    Component.onCompleted: {
        content0.createObject(ch0.contentParent)
        content1.createObject(ch1.contentParent)
        content2.createObject(ch2.contentParent)
        content3.createObject(ch3.contentParent)
        content4.createObject(ch4.contentParent)
        ch0.roundButton.toggle()
    }
    Button{
        id:nextpage
        visible: uiTest
        anchors.right: parent.right
        onClicked: {
            nextstage()
        }
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
