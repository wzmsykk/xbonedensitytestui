import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3

Rectangle{
    id:inputPanel
    width: parent.width*0.4
    property Item backgI
    property Item target
    property string chars: ""
    clip: true
    property int gridSpacing:6
    property int tileWidth: (inputPanel.width-2*gridSpacing)/3
    property int tileHeight: (inputPanel.height-3*gridSpacing)/4
    function applyKey(inkey){
        if(target!=null){
            if(inkey==="\u232B")
            {target.remove(target.length-1,target.length)}
               else if(inkey==="Next"){
            }
            else{ target.text+=inkey }
        }
        console.log(inkey)
    }

    transitions: Transition {
        AnchorAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    Rectangle{
        id:backgr
    color: "white"
    opacity: 0.74
    anchors.fill: parent
    FastBlur {
        id: blur
        source: backgI
        width: source.width
        height: source.height
        radius: 64
        //transparentBorder :true
    }
    }
    Grid{
        columns: 3
        rows:4
        spacing: gridSpacing
        Repeater{
            model:["1","2","3","4","5","6","7","8","9","Next","0","\u232B"]
            delegate: PanelTile{
                text: modelData.toString()
                onTileClicked: {
                    chars=text

                    applyKey(chars)
                }
            }
        }



    }


    onXChanged: {
        setBlurPosition()
    }
    onYChanged: {
        setBlurPosition()
    }
    function setBlurPosition() {
        blur.x = backgI.x - x
        blur.y = backgI.y - y
    }
    Behavior on x {
        NumberAnimation {
        }
    }
    Behavior on y {
        NumberAnimation {
        }
    }
}
