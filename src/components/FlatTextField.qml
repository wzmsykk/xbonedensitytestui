import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
Rectangle{
    id: rectangle

    property alias content: ftf
    property alias title: label.text
    property string types: "text"
    property var model: [ftf.text]
    property int index: 0
    property alias validator: ftf.validator
    signal focused()
    implicitWidth: 200
    implicitHeight: 100
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    opacity: style==="simple"?(ftf.activeFocus?"1":"0.8"):(ftf.activeFocus?"0.6":"0.4")
    function forceFocus(){
        ftf.forceActiveFocus()

    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            ftf.focus=true
            mouse.accepted=true
        }
    }

    ColumnLayout{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
       height: parent.height
    Label {
        id: label

        width: parent.width*0.4
        height: parent.height*0.2
        text: "placeTxe"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 18
        fontSizeMode: Text.Fit
    }

    TextField{

        id:ftf
        height: parent.height*0.4
        font.pixelSize: 40
        color: primaryColor
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        cursorVisible: false
        onActiveFocusChanged: {
            if(ftf.activeFocus===true)
                focused()
        }

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            border.color: "transparent"
            color: "transparent"


        }
        validator: IntValidator{
            bottom: 0
            top:99
        }

    }
}
}
