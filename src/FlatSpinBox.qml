import QtQuick 2.7

import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle{
    id:spinBox
    property string types: "selection"
    property Item content: spinBox
    property alias title: label.text
    property int index: 0
    signal focused()
    property var model:[]
    implicitWidth: 200
    implicitHeight: 100
    border.width: style==="simple"?1:0
    border.color: style==="simple"?primaryColor:"transparent"
    function forceFocus(){
        spinBox.forceActiveFocus()
        focused()
    }
    opacity:  style==="simple"?(spinBox.activeFocus?"1.0":"0.8"):(spinBox.activeFocus?"0.6":"0.4")


    ColumnLayout{
        anchors.fill:parent
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
    RowLayout{
        anchors.bottom: parent.bottom

        Rectangle{
        width: spinBox.width*0.1
        height: spinBox.height*0.7
        border.width: 1
        color: "grey"
        MouseArea{
            anchors.fill: parent
            onPressed: {
                if(index>0) index--
            }
        }
        }
    Label{
        id:content
        width: spinBox.width*0.8
        height: spinBox.height*0.7
         anchors.horizontalCenter: parent.horizontalCenter
         text:model[index]
         horizontalAlignment: Text.AlignHCenter
         Layout.fillWidth: true
         font.pixelSize: 22
         fontSizeMode: Text.Fit

    }
    Rectangle{
    width: spinBox.width*0.1
    height: spinBox.height*0.7
     border.width: 1
     color: "grey"
     MouseArea{
         anchors.fill: parent
         onPressed: {
             if(index<model.length-1)index++
         }
     }
    }

    }
}

}
