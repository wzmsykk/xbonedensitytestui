import QtQuick 2.10
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Layouts 1.0
import QtQuick.Controls.Material 2.3
FluidControls.Card {
    id:card2
    property int elevation: 2
    Material.elevation:elevation
    width: cardWidth
    height: 0.9 * page.height
    Material.elevation: 2
    MouseArea{
        anchors.fill:parent
        hoverEnabled: true
        onEntered: {
            card2.elevation=6
        }
        onExited: {
            card2.elevation=2
        }
        onPressed: {
            card2.elevation=2
            stackView.push("Preference.qml")
            
        }
    }
    Image {
        id: picture2
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: 0.6 * parent.height
        source: "FrantPageReanalize.jpg"
    }
    
    Column {
        id: column2
        anchors {
            left: parent.left
            top: picture2.bottom
            right: parent.right
            margins: FluidControls.Units.smallSpacing * 2
        }
        spacing: FluidControls.Units.smallSpacing * 2
        
        FluidControls.TitleLabel {
            text: qsTr("Reanalize")
        }
        
        FluidControls.BodyLabel {
            text: qsTr("Reanalize the Last test")
            wrapMode: Text.WordWrap
            width: parent.width
        }
    }
}
