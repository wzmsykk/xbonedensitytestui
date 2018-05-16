import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import imageEdit 1.0
Grid {
    id: lb02
    columns: 3
    anchors.fill: parent
    spacing: 6
    padding: 6
    ImageEdit{
        id:ige
    }

    Image {
        id: boneImage
        cache: false
        source: resultReady?"file:./"+"test2.bmp":"pic/tmp.jpg"
        width: parent.width / 2-6
        height: parent.height-12
    }

    Text{

         width: parent.width / 2-6
         height: parent.height-12
       wrapMode: Text.Wrap
        text:qsTr("Result")+ qsTr("\nOperator:")+currInfo[0]+qsTr("\nPatientAge:")+currInfo[1]+qsTr("\nGender:")+currInfo[2]+qsTr("\nRace:")+currInfo[3]+qsTr("\nBone density#1 \n#2 \n#3 \nAverange \nT \nZ ")


    }
    Component.onCompleted: {
        recover();

    }
    function recover(){
        cancelButton.state="idle"
        thirdButton.state="idle"
        acceptAction10.enabled=true
        backAction10.enabled=true
        thirdAction10.enabled=true
        popupLoader.source=""
        popupLoader.state="hide"
        inputPanel.state="hide"
        ed.enabled=false
        fg.enabled=false
        acceptButton.text=qsTr("Edit")
        cancelButton.text=qsTr("Back")
        thirdButton.text=qsTr("Selection")
    }

    Connections {
        id: thirdAction10
        target: thirdButton
        enabled: false
        onButtonClicked: {
            popupLoader.source="FingerSettings.qml"
            popupLoader.item.getFingerVisibles(currentFingerSelection)
            popupLoader.state="show2"
            inputPanel.state="show"
            fg.enabled=true
            thirdButton.state="invisible"
            thirdAction10.enabled=false
            backAction10.enabled = false
            acceptAction10.enabled = false
        }
    }
    Connections{
        id:fg
        target: popupLoader.item
        ignoreUnknownSignals: true
        enabled:false
        onAccepted:{
            currentFingerSelection=result
            ige.changeLCD(result)
            boneImage.source=""
            boneImage.source=resultReady?"file:./"+"test2.bmp":"pic/tmp.jpg"
           recover()

        }
        onCanceled:{
           recover()
        }
    }
    Connections {
        id: acceptAction10
        target: acceptButton
        enabled: false
        onButtonClicked: {
            //anchorScript.state = "normal"
            acceptAction10.enabled=false
            backAction10.enabled=false
             thirdAction10.enabled=false
            popupLoader.source="PatientSet.qml"
            popupLoader.state="show2"
            inputPanel.state="show"
            ed.enabled=true
        }
    }
    Connections{
        id:ed
        target: popupLoader.item
        ignoreUnknownSignals: true
        enabled:false
        onAccepted:{
           recover()
            currInfo=infoSet
        }
        onCanceled:{
           recover()
        }
    }

    Connections {
        id: backAction10
        target: cancelButton
        enabled: false
        onButtonClicked: {
            anchorScript.state = "normal"
            backAction10.enabled=false
            acceptAction10.enabled=false
             thirdAction10.enabled=false
        }
    }
}
