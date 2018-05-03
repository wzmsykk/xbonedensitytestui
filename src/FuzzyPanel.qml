import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Rectangle {
    id:pane
    property Item target
    clip: true
    property int titleHeight: pane.height*0.14
    property alias titleBar: titleBar
    property alias titleColor: titleBar.color
    property alias titleText: titleText
    signal panelClicked()

    state:"normal"


    MouseArea{
        anchors.fill:parent
        enabled: pane.state==="normal"
        onPressed: {
            backgr.opacity=0.3
        }
        onReleased: {
            backgr.opacity=0.7
        }

        onClicked: {

            panelClicked()
        }
    }
    Rectangle{
        id:backgr
        color: "white"
        anchors.fill: parent
    opacity: 0.74
    FastBlur {
        id: blur
        source: target
        width: source.width
        height: source.height
        radius: 64
        //transparentBorder :true
    }
    }
    Rectangle {
        id: titleBar
        //height: titleHeight
        height:0
        width: parent.width
        opacity: 0.86

        visible: false//temp invisible
        Rectangle{
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            color: "transparent"
        Text {

            id: titleText
            color: "#ffffff"
            text: "MyText"
            anchors.left: parent.left
            font.bold: false
            font.weight: Font.Normal
            font.pointSize: 40
            fontSizeMode: Text.VerticalFit
            height: parent.height
            width: titleText.paintedWidth

        }
        }
    }


    onXChanged: {
        setBlurPosition()
    }
    onYChanged: {
        setBlurPosition()
    }
    function setBlurPosition() {
        blur.x = target.x - x
        blur.y = target.y - y
    }
    Behavior on x {
        NumberAnimation {
        }
    }
    Behavior on y {
        NumberAnimation {
        }
    }
}
