import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Grid {
    anchors.fill: parent
    columns: 1
    rows: 3
    spacing: 6
    padding: 6
    signal accepted
    signal canceled
    FlatSpinBox {
        id: ps01
        title: qsTr("Copies")
        model: [1, 2, 3, 4, 5, 6, 7, 8, 9]
        width: parent.width - 2 * padding
    }
    FlatSpinBox {
        id: ps02
        title: qsTr("Color")
        model: ["colored", "grey"]
        width: parent.width - 2 * padding
    }
    FlatSpinBox {
        id: ps03
        title: qsTr("DPi")
        model: ["high", "low"]
        width: parent.width - 2 * padding
    }
    Component.onCompleted: {
        ps01.index = printSet[0]
        ps02.index = printSet[1]
        ps03.index = printSet[2]
        cancelButton.state = "idle"
    }
    Connections {
        id: accept
        target: acceptButton
        enabled: true
        onButtonClicked: {
            printSet[0] = ps01.index
            printSet[1] = ps02.index
            printSet[3] = ps03.index
            accepted()
            accept.enabled = false
        }
    }
    Connections {
        id: cancel
        target: cancelButton
        enabled: true
        onButtonClicked: {
            canceled()
            cancel.enabled = false
        }
    }
}
