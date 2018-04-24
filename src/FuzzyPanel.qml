import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Rectangle {
    id:pane
    property Item target
    property int titleHeight: pane.height*0.14
    property alias titleBar: titleBar
    property alias titleColor: titleBar.color
    property alias titleText: titleText
    clip: true

    state: "normal"
    transitions: Transition {
        AnchorAnimation {
            duration: 200
           easing.type: Easing.InOutQuad
        }
    }
    MouseArea{
        anchors.fill:parent
        onPressed: {
            backgr.opacity=0.3
        }
        onReleased: {
            backgr.opacity=0.7
        }

        onClicked: {

            if(parent.state==="normal")
            {parent.state="expand"}else{
                parent.state="normal"
            }
        }
    }
    Rectangle{
        id:backgr
    color: "white"
    opacity: 0.74
    anchors.fill: parent
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
        height: titleHeight
        width: parent.width
        opacity: 0.86
        Text {

            id: titleText
            color: "#ffffff"
            text: "MyText"

            font.bold: false
            font.weight: Font.Normal
            font.pointSize: 40
            fontSizeMode: Text.VerticalFit
            height: parent.height

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
