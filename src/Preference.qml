import QtQuick 2.7
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
Page {
    id:page
    title:qsTr( "Preferences")
    property int cardWidth:(page.width-FluidControls.Units.smallSpacing * 8)/3
    Row {
        anchors.top:parent.top
        topPadding: FluidControls.Units.smallSpacing * 2
        leftPadding:FluidControls.Units.smallSpacing * 2
        spacing: FluidControls.Units.smallSpacing * 2


        Card1 {
            id: card1
        }
        Card1 {
            id: card2
            titleText: qsTr("SetTime")
            bodyText: qsTr("SetTime")
        }
        Card1 {
            id: card3
            titleText: qsTr("Preference")
            bodyText: qsTr("Preference & Settings")
        }
        Connections{
            target: card2
            onPressed:{
                timeD.open()
            }
        }
    }
    FluidControls.DateTimePickerDialog{
        id:timeD

        onAccepted: {
                      console.log(timeD.selectedDateTime);
                  }
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
    }
}
