import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
ApplicationWindow {
id:root
width: 800
height: 500
 visible: true
  title: qsTr("Test App")
property int paneHeight: 200
property int paneWidth: 300
property int vMargin: 10
property int hMargin: 10
property int toolBarHeight: 0.14*root.height
background: Rectangle {
    id: backRec
    anchors.fill: parent
    Image {
        id: backGImage
        source: "pic/timg.jpg"
        anchors.fill:parent
        //sourceSize: Qt.size(parent.width,parent.height)
        fillMode:Image.Stretch
        smooth: true
        visible: false

    }

    GaussianBlur {
        id:backGBlur
              anchors.fill: backGImage
              source: backGImage
              radius: 64
              samples: 32
          }
}

    Item{
        id:centPoint
        x:parent.width/2
        y:(parent.height-toolbar.height-vMargin)/2
    }
    Rectangle{
        id:toolbar
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: vMargin
        anchors.left: parent.left
        anchors.leftMargin: hMargin
        anchors.right: parent.right
        anchors.rightMargin: hMargin
        height:toolBarHeight
        Label {
            id: timeText
            text: new Date().toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
            color: "white"
            opacity: 0.7
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            fontSizeMode: Text.VerticalFit
            minimumPixelSize: 10
            font.pixelSize: 72
        }
    }

FuzzyPanel {
    id: lefttop
    anchors.left: parent.left
    anchors.bottom: centPoint.top
    anchors.right: centPoint.left
    anchors.top: parent.top

    titleHeight: 45
    titleColor: "#03A9F4"
    titleText.text: qsTr("Start")
    target:backGImage
    function changeAnchorToExpand(){
        height=undefined
        width=undefined
        anchors.bottom= parent.bottom
        anchors.bottomMargin= vMargin
        anchors.right= parent.right
        anchors.rightMargin=hMargin
    }
    states: [
        State {
            name: "expand"
            AnchorChanges {
                target: lefttop.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges{
                target: leftbottom
                anchors.bottom: undefined
                anchors.left: undefined
                anchors.top: parent.bottom
                anchors.right: parent.right
            }
            AnchorChanges{
                target: righttop
                anchors.top: undefined
                anchors.right: undefined
                anchors.bottom: parent.bottom
                anchors.left: parent.right
            }
            AnchorChanges{
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.bottom
                anchors.left: parent.right
            }

            AnchorChanges{
                target: lefttop
                anchors.left:parent.left
                anchors.top:parent.top
                anchors.bottom:toolbar.top
                anchors.right: parent.right

            }
            PropertyChanges {
                target: lefttop
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin
                anchors.rightMargin: vMargin
            }


        },
        State {
            name: "normal"
            AnchorChanges {
                target: lefttop.titleText
                anchors.horizontalCenter: undefined
                anchors.left: parent.left
            }
            AnchorChanges{
                target:lefttop
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: centPoint.top
                anchors.right: centPoint.left
            }
            PropertyChanges {
                target: lefttop
                anchors.leftMargin: hMargin
                anchors.topMargin: vMargin
                anchors.bottomMargin: hMargin/2
                anchors.rightMargin: vMargin/2
            }


        }
    ]


}
FuzzyPanel{
    id:leftbottom
    anchors.bottom:toolbar.top
    anchors.left: parent.left
    anchors.top: centPoint.top
    anchors.right: centPoint.left
    anchors.leftMargin: hMargin
    anchors.topMargin: vMargin/2
    anchors.bottomMargin: vMargin
    anchors.rightMargin: hMargin/2
    titleHeight: 45
    titleColor: "#03A9F4"
    target:backGImage
    states: [
        State {
            name: "expand"
            AnchorChanges {
                target: leftbottom.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges{
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom:parent.top
                anchors.right: parent.right
            }
            AnchorChanges{
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.top
                anchors.left: parent.right
            }
            AnchorChanges{
                target: leftbottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom:toolbar.top

                anchors.right: parent.right

            }
            AnchorChanges{
                target: righttop
                anchors.right: undefined
                anchors.top:undefined
                anchors.bottom:parent.top

                anchors.left: parent.right

            }


        },
        State {
            name: "normal"
            AnchorChanges {
                target: leftbottom.titleText
                anchors.horizontalCenter: undefined
                anchors.left: parent.left
            }
            AnchorChanges{
                target:leftbottom
                anchors.bottom:toolbar.top
                anchors.left: parent.left
                anchors.top: centPoint.top
                anchors.right: centPoint.left
            }


        }
    ]
}
FuzzyPanel{
    id:righttop
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: centPoint.top
    anchors.left: centPoint.left
    anchors.leftMargin: hMargin/2
    anchors.topMargin: vMargin
    anchors.bottomMargin: vMargin/2
    anchors.rightMargin: hMargin
    titleHeight: 45
    titleColor: "#03A9F4"
    target:backGImage
    states: [
        State {
            name: "expand"
            AnchorChanges {
                target: righttop.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges{
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom:  parent.bottom
                anchors.right: parent.left
            }
            AnchorChanges{
                target: rightbottom
                anchors.bottom: undefined
                anchors.right: undefined
                anchors.top: parent.bottom
                anchors.left: parent.left
            }
            AnchorChanges{
                target: leftbottom
                anchors.left:undefined

                anchors.bottom: undefined
                anchors.top: parent.bottom
                anchors.right: parent.left

            }
            AnchorChanges{
                target: righttop
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom:toolbar.top

                anchors.right: parent.right

            }


        },
        State {
            name: "normal"
            AnchorChanges {
                target: righttop.titleText
                anchors.horizontalCenter: undefined
                anchors.left: parent.left
            }
            AnchorChanges{
                target: righttop
                anchors.bottom: centPoint.top
                anchors.left: centPoint.left
                anchors.right: parent.right
                anchors.top: parent.top
            }


        }
    ]

}
FuzzyPanel{
    id:rightbottom
    anchors.right: parent.right
    anchors.top: centPoint.top
    anchors.left: centPoint.left
    anchors.bottom:toolbar.top
    anchors.leftMargin: hMargin/2
    anchors.topMargin: vMargin/2
    anchors.bottomMargin: vMargin
    anchors.rightMargin: hMargin
    titleHeight: 45
    titleColor: "#03A9F4"
    target:backGImage
    states: [
        State {
            name: "expand"
            AnchorChanges {
                target: rightbottom.titleText
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
            AnchorChanges{
                target: lefttop
                anchors.left: undefined
                anchors.top: undefined
                anchors.bottom:  parent.top
                anchors.right: parent.left
            }
            AnchorChanges{
                target: righttop
                anchors.bottom: parent.top
                anchors.right: undefined
                anchors.top: undefined
                anchors.left: parent.left
            }
            AnchorChanges{
                target: leftbottom
                anchors.left:undefined
                anchors.top: parent.top
                anchors.bottom: undefined

                anchors.right: parent.left

            }
            AnchorChanges{
                target: rightbottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom:toolbar.top

                anchors.right: parent.right

            }


        },
        State {
            name: "normal"
            AnchorChanges {
                target: rightbottom.titleText
                anchors.horizontalCenter: undefined
                anchors.left: parent.left
            }
            AnchorChanges{
                target: rightbottom
               anchors.bottom:toolbar.top

                anchors.right: parent.right
                anchors.top: centPoint.top
                anchors.left: centPoint.left
            }
        }
    ]
}
}

