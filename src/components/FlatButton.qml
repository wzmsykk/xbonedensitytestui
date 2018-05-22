import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
Rectangle{
    id: ftb
    property alias content: ftb
    property alias title: label.text
    property string types: "text"
    signal buttonClicked()
    implicitWidth: 200
    implicitHeight: 100
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    opacity: style==="simple"?(ftb.activeFocus?"1":"0.8"):(ftb.activeFocus?"0.6":"0.4")
    function forceFocus(){
        ftb.forceActiveFocus()

    }
    states: [State {
            name: "idle"
            PropertyChanges {
                target: object

            }
        }]
    MouseArea{
        anchors.fill: parent
        onClicked: {
            ftb.focus=true
            mouse.accepted=true
             buttonClicked()
        }
        onPressed: {
            if(style==="simple")ftb.opacity=0.3
        }
        onReleased: {
             if(style==="simple")ftb.opacity=0.65
        }


    }

    ColumnLayout{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

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


}
}
