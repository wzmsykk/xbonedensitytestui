import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle {
    id: acceptButton
    color: "transparent"
    implicitHeight: 40
    implicitWidth: 60
    state: "invisible"
    signal buttonClicked()
    Behavior on opacity {NumberAnimation{}}
    states: [
        State {
            name: "idle"
            PropertyChanges {
                target: acceptButton
                visible:true
                opacity:1

            }
            PropertyChanges {
                target: backtangle
                color:"#4CAF50"
                opacity:1
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity: 1
            }
        },State {
            name: "pressed"
            PropertyChanges {
                target: backtangle
                color:"#9E9E9E"
                opacity:0.8
            }
            PropertyChanges {
                target: label1
                color: "white"
                opacity: 1
            }
        },State {
            name: "invisible"
            PropertyChanges {
                target: acceptButton
                visible:false
                opacity:0

            }
        }]
    Rectangle{
        id:backtangle
        anchors.fill: parent
        color: "#607D8B"
        opacity: 0.8
    }
    Label{
        id:label1
        text: qsTr("Accept")
        color: "white"
        anchors.fill: acceptButton
        anchors.margins: 8
        fontSizeMode: Text.VerticalFit
        minimumPixelSize: 10
        font.pixelSize: 72
        opacity: 0.8
    }

    MouseArea{
        id:acceptAction
        anchors.fill: parent
        enabled: parent.visible
        onPressed: {
            acceptButton.state="pressed"
        }
        onClicked: {
            buttonClicked()
        }
        onReleased: {
            acceptButton.state="idle"
        }
    }
}
