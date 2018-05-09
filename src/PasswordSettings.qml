import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Grid{
    anchors.fill: parent
    columns: 1
    rows:3
    spacing: 6
    padding: 6
    signal accepted()
    signal canceled()
    FlatPasswordTextField{
        id:ps01
        title: qsTr("OldPassword")

        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps01
        }
    }
    FlatPasswordTextField{
        id:ps02
        title: qsTr("NewPassword")

        width: parent.width-2*padding
        onFocused: {
            inputPanel.targetDist=ps02
        }
    }

    Component.onCompleted: {
        inputPanel.role="numberial"
            inputPanel.targetDist=ps01

          cancelButton.state="idle"

    }
    Component.onDestruction: {
             inputPanel.setDefaultDist()
    }

    Connections{
        id:accept
        target: acceptButton
        enabled:true
        onButtonClicked:{

              accepted()
            accept.enabled=false
        }
    }
    Connections{
        id:cancel
        target: cancelButton
        enabled:true
        onButtonClicked:{

            canceled()
            cancel.enabled=false
        }
    }


}
