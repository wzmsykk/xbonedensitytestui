import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0 as FluidControls
ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    //color: "#fafafa"
    title: qsTr("Test App")
    FluidControls.SnackBar {
        id: snackBar
   }
    Action{
        id:back
        shortcut: "Esc"
        onTriggered: {
            if(stackView.depth>1)stackView.pop();
        }
    }

    Action {
        id:specialSetShortCut
    shortcut:"Ctrl+S"
    onTriggered: {
        stackView.push("AnimationTest2.qml")
        specialSetShortCut.enabled=false
        }
    }

    header: ToolBar {
        id: toolBar
        property var locales: Qt.locale()
        contentHeight: backButton.implicitHeight
        RowLayout {
            anchors.fill: parent

            Label {
                text: new Date().toLocaleTimeString(ToolBar.locales)
            }

            Label {
                text: stackView.currentItem.title
                horizontalAlignment: Text.AlignHCenter
                textFormat: Text.AutoText
                anchors.centerIn: parent


                Layout.fillWidth: true
            }

            ToolButton {
                id: backButton
                visible:  stackView.depth > 1 ? true : false
                text: stackView.depth > 1 ? "\u25C0" : "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                focusPolicy: Qt.NoFocus

                onClicked: {
                    if(specialSetShortCut.enabled===false){
                        specialSetShortCut.enabled=true
                    }

                    if (stackView.depth > 1) {
                        stackView.pop()
                    } else {
                        drawer.open()
                    }
                }
            }
        }
    }
    Drawer {
        id: drawer
        width: root.width * 0.66
        height: root.height
        edge: Qt.RightEdge
        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Page 1")
                width: parent.width
                onClicked: {
                    stackView.push("PersonalInfo.qml")

                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Page 2")
                width: parent.width
                onClicked: {
                    stackView.push("Preference.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "LoadPage.qml"
        anchors.fill: parent


    }


}
