import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle{
    id:u3
    property string types: "selection"
    property alias title: label.text
    property int index: 0
    signal focused()
    property var model
    implicitWidth: 200
    implicitHeight: 100


    opacity: u3.activeFocus?"0.6":"0.4"
    MouseArea{
        anchors.fill: parent
        onClicked: {
            u3.focus=true
            focused()
            mouse.accepted=true
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
        font.pointSize: 12
        //fontSizeMode: Text.Fit
    }
    Label{
        id:content
        width: parent.width*0.8
        height: parent.height*0.4
         anchors.horizontalCenter: parent.horizontalCenter
         text:model[index]
         font.pixelSize: 22
         fontSizeMode: Text.Fit
    }


}

}
