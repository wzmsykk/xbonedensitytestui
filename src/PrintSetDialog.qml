import QtQuick 2.7
import QtQuick.Controls 2.3
import Fluid.Controls 1.0 as FluidControls
import com.kp.settings 1.0
import QtQuick.Controls.Material 2.3
Dialog {
    id: printSetDialog
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    title: qsTr("Print Settings")
    Material.elevation: 24

    standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
    onOpened: {
        mySet.readSettings()
        list1.clear()

        list1.append({
                         key: "low",
                         value: Settings.LowDpi
                     })
        list1.append({
                         key: "high",
                         value: Settings.HighDpi
                     })
        cpText.text = mySet.copies.toString()
        comboBox.currentIndex = (mySet.dpi === Settings.LowDpi) ? 0 : 1
        comboBox1.currentIndex = (mySet.color === false) ? 0 : 1
    }

    onAccepted: {

        mySet.writeSettings()
        snackBar.open(qsTr("Print Settings Changed."))
    }

    Column {
        id: column
        width: 200
        height: 400

        Label {
            id: label
            text: qsTr("Copies:")
        }

        TextField {
            id: cpText
            text: ""
            validator: IntValidator {
                bottom: 1
                top: 999
            }
            onFocusChanged: {
                if(cpText.focus===true) cpText.selectAll();
            }
            onTextChanged: {
                mySet.copies=cpText.text;
            }
        }

        Label {
            id: label1
            text: qsTr("Dpi:")
        }

        ComboBox {
            id: comboBox
            textRole: "key"
            model: list1
            onActivated: {
                if (comboBox.currentIndex === 0)
                    mySet.dpi = Settings.LowDpi
                else
                    mySet.dpi = Settings.HighDpi
                console.log(Settings.LowDpi)
            }
        }

        Label {
            id: label2
            text: qsTr("Colored:")
        }

        ComboBox {
            id: comboBox1
            textRole: "key"
            model: list2
            onActivated: {
                if (comboBox1.currentIndex === 0)
                    mySet.color = false
                else
                    mySet.color = true
            }
        }
    }
}

