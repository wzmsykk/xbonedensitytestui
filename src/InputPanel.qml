import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import "./components"
Rectangle{
    id:inputPanel
    width: parent.width*0.4
    property Item backgI
    property Item targetDist:defDist
    property string chars: ""
    clip: true
    function setDefaultDist(){
        inputPanel.targetDist=defDist
        inputPanel.role="numberial"
    }

    Item{
        id:defDist
    }
    property string role: "numberial"


    property int gridSpacing:6
    property int tileWidth: (inputPanel.width-2*gridSpacing)/3
    property int tileHeight: (inputPanel.height-3*gridSpacing)/4
    function applyKey(inkey){

        if(targetDist!=null){
            if(targetDist.enabled===false)  return 0
            if(inkey==="\u232B")
            {targetDist.content.remove(targetDist.content.length-1,targetDist.content.length)}

            else{
                targetDist.content.text+=inkey
                if(targetDist.content.acceptableInput===false) {
                    targetDist.content.remove(targetDist.content.length-1,targetDist.content.length)
                }
            }
        }
        console.log(inkey)
    }
    function applyModel(index){
        if(targetDist!=null){
            targetDist.index=index
        }
    }

    state: "hide"
    states: [State {
            name: "show"
            AnchorChanges{
                target: inputPanel
                anchors.left: undefined
                anchors.right: parent.right
            }
        },State {
            name: "hide"
            AnchorChanges{
                target: inputPanel
                anchors.right: undefined
                anchors.left: parent.right
            }

        }]
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
        visible:style==="simple"?false:true
        //transparentBorder :true
    }
    }
    Grid{
        columns: 3
        rows:4
        spacing: gridSpacing
        id:numgrid
        visible: role==="numberial"?true:false
        Repeater{
            model:["1","2","3","4","5","6","7","8","9","-","0","\u232B"]
            delegate: PanelTile{
                text: modelData.toString()
                onTileClicked: {
                    chars=text

                    applyKey(chars)
                }
            }
        }
    }
    Column{
        id:sele
        spacing: gridSpacing
        visible: role==="selection"?true:false
        Repeater{
            model: (targetDist!=defDist&&targetDist.types==="selection")?targetDist.model:undefined
            delegate: PanelTile{
                width: (inputPanel.width-2*gridSpacing)
                height: (inputPanel.height-3*gridSpacing)/4

                text: modelData.toString()
                onTileClicked: {
                    applyModel(index)
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
