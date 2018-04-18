import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3


//USED in InfoCom
Rectangle {
    id: ch0
    default property alias children: area1.children
    property int visibleHeight: 40
    property int bannerHeight: 40
    property alias contentHeight: area1.height
    property alias labelText: label.text
    property alias contentParent: area1
    property alias contentVisible: area1.visible
    property alias roundButton: rb1
    property alias buttonText: rb1.text
    signal rbCheckedChanged(var checked)
    //width: parent.width
    width: 200
    height: visibleHeight+contentHeight
    Grid {
        id: grid
        spacing: 5
        columns: 2
        rows: 2
        width: parent.width
        flow: Grid.LeftToRight
        RoundButton {
            id: rb1
            height: bannerHeight
            width: bannerHeight
            text: ""
            checkable: true

            //Material:
            onCheckedChanged: {
                if (rb1.checked === true) {
                    rbCheckedChanged(true)
                    contentVisible=true

                } else {
                    rbCheckedChanged(false)
                    contentVisible=false
                }
            }

        }
        Rectangle{
            height: bannerHeight
            width: parent.width-rb1.width
        Label {
            id: label
            height: bannerHeight

            text: ""
            horizontalAlignment: Text.AlignLeft
            renderType: Text.QtRendering
            verticalAlignment: Text.AlignVCenter
            textFormat: Text.PlainText
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        }
        Rectangle {
            color: "transparent"
            width: rb1.width
            height: 1
        }
        Rectangle {
            id: area1
            width: parent.width - bannerHeight
            height: 60
            color: "transparent"
            onChildrenChanged: {

                area1.height=children[0].height
                console.log(area1.height)
            }
        }
    }
}
