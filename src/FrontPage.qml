import QtQuick 2.10
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Layouts 1.0
import QtQuick.Controls.Material 2.3
Page {
    id:page
    title: qsTr("XRayBoneDensityTools")
    signal buttonPressed(int a);
    property int cardWidth:(page.width-FluidControls.Units.smallSpacing * 8)/3
        Rectangle{
            anchors.fill: parent
            color: "#fafafa"
            z:-1
        }

    Row {
        anchors.top:parent.top
        topPadding: FluidControls.Units.smallSpacing * 2
        leftPadding:FluidControls.Units.smallSpacing * 2
        spacing: FluidControls.Units.smallSpacing * 2


        Card1 {
            id: card1
            titleText: qsTr("Start")
            bodyText: qsTr("Start up")
            onPressed: stackView.push("PersonalInfo.qml")
        }
        Card1 {
            id: card2
            titleText: qsTr("ReAnalize")
            bodyText: qsTr("reanalize")
        }
        Card1 {
            id: card3
            titleText: qsTr("Preference")
            bodyText: qsTr("Preference & Settings")
        }
        Connections{
            target: card3
            onPressed:{
                stackView.push("Preference.qml")
            }
        }
    }

}
