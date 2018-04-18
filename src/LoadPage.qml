import QtQuick 2.0
import QtQuick.Controls 2.3
Page {
    id:pg
    title: "L"
    Loader{
        id:ld
        anchors.centerIn: parent
        source: "initq.qml"
        height: parent.height*0.4
        width: parent.width*0.6
    }
    Connections{
        target: ld.item
        onInitAllSucceed:{
            stackView.pop()
        }
    }
}
