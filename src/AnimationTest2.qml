import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import Fluid.Controls 1.0 as FluidControls
import Fluid.Layouts 1.0
Page {
    id:page
    property int cardWidth:(page.width-FluidControls.Units.smallSpacing * 8)/3
    title: "Animationtest2"
    Card1{
        id: card1
        width: cardWidth
        height: 0.9*page.height
        titleText: qsTr("Start")
        bodyText: qsTr("Start up")
    }
    Loader{
        source: "InitBox.qml"
        height: 200
        width:200
    }

    Item {
        id: aa
        state: "B"
        states: [
            State {
                name: "A"
                PropertyChanges {
                    target: aa
                    visible: false
                }
            },
            State {
                name: "B"
                PropertyChanges {
                    target:aa
                    visible:true
                }
            }
        ]
        Button{
            height: 60
            width: 60
            onPressed: {
                aa.state="A"
            }
        }
    }
}
