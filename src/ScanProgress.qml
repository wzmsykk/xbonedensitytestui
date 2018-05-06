import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.kp.scan 1.0
Page {
    id:progressPage

    Scan{
        id:sc
        onHandleScanPreperationResults:{
            if(result===1)
            {
                prog.value=1
                console.log("pared.")
            }
        }
        onHandleScanResults: {
            if(result===1){
                 text2.text=qsTr("Getting digital Image")
                prog.value=2
            }else if(result===2){
                 text2.text=qsTr("Confirming bone area.")
                prog.value=3
            }else if(result===3){
                text2.text=qsTr("Calculating bone density.")
                prog.value=4
            }else if(result===4){
                 text2.text=qsTr("Calculating T-Z value.")
                prog.value=5
            }else if(result===5){
                comp1.createObject(progressPage)
            }
        }
    }
    Component{
        id:comp1
        Rectangle{
            id:cc
            anchors.fill: parent
            color: "#fafafa"
        }
    }

    Dialog{
        id:exitAlert
        modal:true
        x:(parent.width-exitAlert.width)/2
        y:(parent.height-exitAlert.height)/2
        Label{
            text:qsTr("Really to stop the examination?")
        }
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            sc.cancelScan();
            stackView.pop();
            backAction.enabled=true
            backButton.visible=true
        }
    }

    Component.onCompleted: {
        backAction.enabled=false
        backButton.visible=false
        sc.prepareScan()
        rectangle.forceActiveFocus()

    }
    Rectangle{
        id: rectangle
        anchors.fill: parent
        color: "Grey"
        focus: true
        Keys.enabled: true
        Keys.onPressed: {
            console.log("keya pressed")
            if(event.key===Qt.Key_A&&prog.value===1)
            {sc.operateScan()
            text2.text=qsTr("Scan ok, move your hands out.")

            }
        }

        Image {
            id: pic1
            source: "/pic/tmp.jpg"
            height: parent.height
            width: 300

        }
       Text{
           id:text2

           text: qsTr("When the green light is on, put the hand on ... then hold the button")
           anchors.left: pic1.right
           anchors.top: parent.top
           anchors.right: parent.right
           wrapMode: Text.WordWrap
           fontSizeMode: Text.FixedSize
       }
    }

    footer: ToolBar{
        id:tb
        width: parent.width
        Material.background:  "white"
        state: "doScan"
        states: [
            State {
                name: "doScan"
                PropertyChanges {
                    target: object

                }
            },State {
                name: "showResult"
                PropertyChanges {
                    target: object

                }
            }
        ]
        RowLayout {
            anchors.fill: parent

        Button{
            id:previous
            text:qsTr("<Back")
            flat: true
             Material.foreground: "black"
            onClicked: {
                exitAlert.open()
            }
        }
       Rectangle{
           Layout.fillWidth: true
         ProgressBar {
             id:prog
             from:0
             to:5
            value: 0
            anchors.centerIn: parent
             width: parent.width*0.7
            Behavior on value {
                NumberAnimation{}
            }

         }
       }
         Button{
             id:next
             text:qsTr("Next>")
             Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
             flat: true
             Material.foreground: "black"
             visible: false

         }
        }
    }

}
