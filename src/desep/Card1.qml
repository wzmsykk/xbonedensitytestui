import QtQuick 2.10
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Layouts 1.0
import QtQuick.Controls.Material 2.3
FluidControls.Card {

    id:card1
    property int elevation: 2
    property string loadQ: ""
    property string picSource: ""
    property string activedPicSource: ""
    property var titleText: qsTr("This is Title")
    property var bodyText: qsTr("This is body")
    property var userCostomKey
    signal pressed()
    focusPolicy: Qt.WheelFocus
    onActiveFocusChanged: {
        Material.elevation=2+activeFocus*4
        picture.source=(activeFocus===0?picSource:activedPicSource)
    }
    Keys.onPressed: {
        if(event.key===Qt.Key_Return|event.key===Qt.Key_Space|event.key===userCostomKey) {card1.pressed()
            event.accepted=true
        card1.focus=false}
    }

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
