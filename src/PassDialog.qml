import QtQuick 2.7
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import com.kp.settings 1.0
import QtQuick.Controls.Material 2.3

Dialog {
    id: passDialog
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    property bool oldVisible: false
    modal: true
    title: qsTr("Password Settings")
    Material.elevation: 24
    standardButtons: DialogButtonBox.Apply | DialogButtonBox.Reset | DialogButtonBox.Cancel

    onOpened: {
        if(mySet.isPswExist===true) {
            oldPass.focus=true
        }else{
            newPass.focus=true
        }
    }
    onReset: {
        if (mySet.isPswExist === true) //needs more safety
        {
            if (mySet.password.toString() === oldPass.text) {
                mySet.password = 0
                mySet.isPswExist = false
                mySet.writeSettings()
                snackBar.open(qsTr("Password Deleted"))
                newPass.clear()
                oldPass.clear()
                passDialog.close()
            } else {
                snackBar.open(qsTr("Wrong old password"))
                newPass.clear()
                oldPass.clear()
            }
        }
    }
    
    onApplied: {
        if (mySet.isPswExist === false) {
            if (newPass.text != "") {
                mySet.password = newPass.text
                mySet.isPswExist = true
                mySet.writeSettings()
                snackBar.open(qsTr("Password Changed"))
                passDialog.close()
            } else {
                snackBar.open(qsTr("Empty Input"))
            }
            
            newPass.clear()
        } else {
            
            if (mySet.password.toString() === oldPass.text) {
                if (newPass.text != "") {
                    mySet.password = newPass.text
                    mySet.writeSettings()
                    snackBar.open(qsTr("Password Changed"))
                    passDialog.close()
                } else {
                    snackBar.open(qsTr("Empty Input"))
                }
                newPass.clear()
                oldPass.clear()
            } else {
                console.log(oldPass.text)
                snackBar.open(qsTr("Wrong old password"))
                newPass.clear()
                oldPass.clear()
            }
        }
    }
    
    Column {
        id: pasCol
        width: 200
        height: 400
        
        Label {
            id: label
            visible: oldVisible
            text: qsTr("Old Password:")
        }

        TextField {
            id: oldPass
            visible: oldVisible
            echoMode: TextInput.Password
            passwordCharacter: "*"
            validator: IntValidator {
                bottom: 0
                top: 9999
            }
        }
        Label {
            id: label1
            text: qsTr("New Password:")
        }

        TextField {
            id: newPass
            echoMode: TextInput.Password
            passwordCharacter: "*"
            validator: IntValidator {
                bottom: 0
                top: 9999
            }
        }


    }
}
