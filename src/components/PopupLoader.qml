import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0

Item{
    id:popupLoader
    width: parent.width-2*hMargin
    anchors.leftMargin: hMargin
    anchors.topMargin: vMargin
    anchors.bottomMargin: hMargin
    anchors.rightMargin: vMargin
    property var item:null
    property int index: 1

    readonly property int maxLayer: 10
    onIndexChanged: {


    }
    onItemChanged: {

    }
    function currItem(){
        console.log(loaderList.itemAt(index).source)
        return loaderList.itemAt(index).item
    }
    function applyState(instate){
        if(instate==="show2") {
            state=instate
            inputPanel.state="show"
        }else if(instate==="show"){
            state=instate
            inputPanel.state="hide"
        }else if(instate===undefined){
            state="show"
        }
        else state=instate
    }

    function push(inUrl,state){

        if(index<=maxLayer)
        {

            index++
            loaderList.itemAt(index).source=inUrl
           item=loaderList.itemAt(index).item
            console.log(inUrl,state)
            if(state!==undefined) {
                applyState(state)
                loaderList.itemAt(index).state=state
            }else{
                applyState("show")
                loaderList.itemAt(index).state="show"
            }

            return 1
        }else return 0
    }
    function pop(){
        if(index>1){

            loaderList.itemAt(index).source=""
            index--
            applyState(loaderList.itemAt(index).state)
            item=loaderList.itemAt(index).item
            console.log("poped")
            return 1

        }else return 0
    }
    function clear(){
        for(var i=index;i>0;i--)        {pop()}
    }


    state:"hide"
    states: [State {
            name: "hide"
            PropertyChanges{
                target: popupLoader
                opacity:0
            }
            AnchorChanges {
                target: popupLoader
                anchors.left:parent.right
                anchors.right:  undefined
            }
        },State {
            name: "show"
            AnchorChanges {
                target: popupLoader
                anchors.left: parent.left
                anchors.right: parent.right
            }
            PropertyChanges{
                target: popupLoader
                opacity:1
            }
        },State {
            name: "show2"
            AnchorChanges {
                target: popupLoader
                anchors.left: parent.left
                anchors.right: inputPanel.left
            }
            PropertyChanges{
                target: popupLoader
                opacity:1
            }

        }]
    Rectangle{
        visible: false
        anchors.fill: parent
        border.color: "green"
    }
    Repeater{
        id:loaderList
        anchors.fill: parent
        model: maxLayer
        delegate: Loader{
            anchors.fill: parent
            source: ""
            z:index
            property string state:"show"

        }
        Component.onCompleted: {
            for(var i=1;i<4;i++)
                console.log(loaderList.itemAt(i))
        }
    }




    transitions: Transition {
        AnchorAnimation{
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
    Behavior on opacity {
        NumberAnimation{}
    }
    Component.onCompleted: {
        item=loaderList.itemAt(index).item
    }

    /*Connections{
        target: popupLoader.item
        ignoreUnknownSignals: true
        onAccepted:{
            popupLoader.pop()
        }
        onCanceled:{
            popupLoader.pop()
        }
    }*/
}

