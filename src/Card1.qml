import QtQuick 2.10
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Layouts 1.0
import QtQuick.Controls.Material 2.3
FluidControls.Card {
    id:card1
    width: cardWidth
    height: 0.9 * page.height
    property int elevation: 2
    property string loadQ: "Preference.qml"
    property string picSource: "FrontPageNewTest.jpg"
    property var titleText: qsTr("Start Up")
    property var bodyText: qsTr("start new test")
    signal pressed()
    Material.elevation:elevation
    MouseArea{
        anchors.fill:parent
        hoverEnabled: true
        onEntered: {
            card1.elevation=6
        }
        onExited: {
            card1.elevation=2
        }
        onPressed: {
            card1.elevation=2
            card1.pressed()
            
        }
    }
    
    Image {
        id: picture
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: 0.6 * parent.height
        source: picSource
    }
    
    Column {
        id:column1
        anchors {
            left: parent.left
            top: picture.bottom
            right: parent.right
            margins: FluidControls.Units.smallSpacing * 2
        }
        spacing: FluidControls.Units.smallSpacing * 2
        
        FluidControls.TitleLabel {
            text: titleText
        }
        
        FluidControls.BodyLabel {
            text: bodyText
            wrapMode: Text.WordWrap
            width: parent.width
        }
    }
}
