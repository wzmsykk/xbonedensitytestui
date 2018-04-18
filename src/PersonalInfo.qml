import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
    id:personInfopage
    contentHeight: 480
    contentWidth: 600
    title: qsTr("Patient Info")
    property var statusArr:[false,false,false,false,false] //


    property var visArr: [true,false,false,false,false]//Try dynamic button generation later
    function update(){
        visArr=visArr
        statusArr=statusArr
    }

        ButtonGroup{
               id:bg1
               buttons: [rb1,rb2,rb3,rb4]

               onCheckedButtonChanged: {

                   for(var i=0;i<buttons.length;i++){


                        if(buttons[i]===checkedButton)
                        {
                          visArr[i]=true


                        }else{
                          visArr[i]=false
                        }
                   }
                   checkedButton.parent.parent.children[1].children[0].children[0].forceActiveFocus()//it Workks
                   update()


               }
        }
    FocusScope{
        GroupBox {
            id: gb1
            anchors.top: parent.top
            background: null

            label: RowLayout {
                RoundButton {
                    id: rb1
                    text:  statusArr[0]? "✔" : "1"
                    spacing: 1
                    checked: true
                    checkable: true
                }
                Label {
                    id: lb1
                    text: qsTr("Operator")
                    //visible: visArr[0]

                }
            }
            ColumnLayout {
                TextField {
                    id: tf1
                    text: ""
                    visible: visArr[0]
                    focus: true
                    Keys.onReturnPressed: {bt1.clicked()
                        console.log("sasa")
                    }
                }
                Button {
                    id:bt1
                    text: "Next"
                    enabled: tf1.text != ""
                    visible: visArr[0]
                    onClicked: {
                        statusArr[0]=true
                         update()
                        rb2.checked=true
                    }
                }
            }
        }
        GroupBox {
            id: gB2
            anchors.top: gb1.bottom



            background: null

            label: RowLayout {
                RoundButton {
                    id: rb2
                    text: statusArr[1] ? "✔" : "2"

                    checkable: true
                }
                Label {
                    id: lb2
                    //visible: visArr[1]
                    text: qsTr("Patient No.")
                }
            }
            ColumnLayout {
                TextField {
                    id: tf2
                    text: ""
                    visible: visArr[1]
                }
                Button {
                    text: "Next"
                    enabled: tf2.text != ""
                    visible: visArr[1]
                    onClicked: {
                        statusArr[1]=true
                         update()
                        rb3.checked=true
                    }
                }
            }
        }

        GroupBox {
            id: gb3
            anchors.top: gB2.bottom
            anchors.leftMargin: 0


            background: null

            label: RowLayout {
                RoundButton {
                    id: rb3
                    text: statusArr[2] ? "✔" : "3"
                    checkable: true

                }
                Label {
                    id: lb3
                    //visible: visArr[2]
                    text: qsTr("Gender")
                }
            }
            ColumnLayout {
                TextField {
                    id: tf3
                    text: ""
                    visible: visArr[2]
                }
                Button {
                    text: "Next"
                    enabled: tf3.text != ""
                    visible: visArr[2]
                    onClicked: {
                       statusArr[2]=true
                         update()
                        rb4.checked=true
                    }
                }
            }
        }
        GroupBox {
            id: gb4
            anchors.top: gb3.bottom
            anchors.leftMargin: 0

            background: null

            label: RowLayout {
                RoundButton {
                    id: rb4
                    text: statusArr[3]? "✔" : "4"
                    checkable: true

                }
                Label {
                    id: lb4
                    //visible: visArr[3]
                    text: qsTr("Race")
                }
            }
            ColumnLayout {
                TextField {
                    id: tf4
                    text: ""
                    visible: visArr[3]
                }
                Button {
                    text: "Next"
                    enabled: tf4.text != ""
                    visible: visArr[3]
                    onClicked: {
                        statusArr[3]=true
                         update()

                    }
                }
            }
        }
}
}
