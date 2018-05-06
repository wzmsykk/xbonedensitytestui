import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0
FuzzyPanel{
    anchors.fill: parent
 signal accepted()
    signal canceled()
    target: backGImage

Column {
    id: lt02
    anchors.fill: parent
    anchors.margins: 20


    spacing: 6
    Label {
        text: qsTr("Info")
    }
    
    FlatTextField {
        id: text1
        title: qsTr("Op ID")
        enabled: true
        focus: true
        width: parent.width
        height: (parent.height - parent.rowSpacing * 3) / 2
        onFocused: {
            inputPanel.targetDist = text1
            inputPanel.state = "numberial"
            ltchildSeq.activeIndex=0
            
        }
    }
    FlatTextField {
        id: text2
        title: qsTr("Patient Age")
        enabled: true
        width: parent.width
        height: (parent.height - parent.rowSpacing * 3) / 2
        onFocused: {
            inputPanel.targetDist = text2
            inputPanel.state = "numberial"
            ltchildSeq.activeIndex=1
        }
    }
    Row {
        spacing: 6
        width: text2.width
        height: text2.height
        FlatSelection {
            id: u3
            width: parent.width/2-3
            height: parent.height
            title: qsTr("Gender")
            model: [qsTr("male"), qsTr("female")]
            onFocused: {
                inputPanel.targetDist = u3
                inputPanel.state = "selection"
                ltchildSeq.activeIndex=2
            }
        }
        FlatSelection {
            id: u4
            width: parent.width/2-3
            height: parent.height
            title: qsTr("Race")
            model: [qsTr("Css"), qsTr("Asian"), qsTr("Others")]
            onFocused: {
                inputPanel.targetDist = u4
                inputPanel.state = "selection"
                
                ltchildSeq.activeIndex=3
            }
        }
    }
    Item{
        id:ltchildSeq
        property var seq: [text1,text2,u3,u4]
        property int activeIndex: 0
        onActiveIndexChanged: {
            if(activeIndex===seq.length-1)
            {acceptButton.text=qsTr("Commit")
                cancelButton.text=qsTr("Previous")}
            else if(activeIndex===0){
                acceptButton.text=qsTr("Next")
                cancelButton.text=qsTr("Cancel")
            }else{cancelButton.text=qsTr("Previous")
                acceptButton.text=qsTr("Next")}
        }
        
        function next(index){
            if(index<seq.length-1){
                seq[index+1].forceFocus()
                activeIndex=index+1
                console.log(activeIndex)}
            else commit()
        }
        function previous(index){
            if(index===0){
                activeIndex=0
                cancelButton.text=qsTr("Cancel")
                pageContent.state="hide"
                anchorScript.state="normal"
                canceled()


            }else{
                seq[index-1].forceFocus()
                activeIndex=index-1
                
            }
            console.log(activeIndex)
        }
        
        function commit()
        {
            for(var i=0;i<seq.length;i++){
                infoSet[0]=seq[0].model[seq[0].index]}
             activeIndex=0
            anchorScript.state="work"
            accepted()
            pageContent.source=""





            
        }
    }
    Component.onCompleted: {
        console.log("pat loaded")
        inputPanel.targetDist=text1
        text1.forceFocus()
        backAction00.enabled=true
        acceptAction00.enabled=true
    }
    Component.onDestruction: {
        inputPanel.setDefaultDist()
        backAction00.enabled=false
        acceptAction00.enabled=false
    }

    Connections {
        id: backAction00
        target: cancelButton
        enabled: false
        onButtonClicked: {
            console.log("ss")
            ltchildSeq.previous(ltchildSeq.activeIndex)
        }
    }
    Connections {
        id: acceptAction00
        target: acceptButton
        enabled: false
        onButtonClicked: {
            ltchildSeq.next(ltchildSeq.activeIndex);
            console.log("acce")
        }
    }
}
}