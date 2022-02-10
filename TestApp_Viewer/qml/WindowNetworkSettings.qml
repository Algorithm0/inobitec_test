import QtQuick
import QtQuick.Controls

Window {
    id: settingWinRoot
    title: "Network Settings"
    width: 380
    height: 385
    maximumHeight: height
    minimumHeight: height
    maximumWidth: width
    minimumWidth: width
    modality: Qt.ApplicationModal
    flags: Qt.Dialog

    Column {
        anchors.fill: parent
        Text {
            id: title
            text: "For the correct operation of the application,\nspecify the following network data:"
            anchors.horizontalCenter: parent.horizontalCenter
            color: "red"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: parent.height * 0.05
        }
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: fieldPortR.parent.width*2.2
            height: fieldPortR.y + fieldPortR.height
            Text {
                text: "Enter port to read:"
                color: "black"
                anchors.left: parent.left
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: title.font.pixelSize * 0.8

                TextField {
                    id: fieldPortR
                    anchors.top: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.left: parent.l
                    placeholderText: networkSettings.portR
                    width: parent.width*0.7
                    validator: IntValidator{bottom: 0; top: 65535;}
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            Text {
                text: "Enter port to write:"
                color: "black"
                anchors.right: parent.right
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: title.font.pixelSize * 0.8

                TextField {
                    id: fieldPortW
                    anchors.top: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    placeholderText: networkSettings.portW
                    width: fieldPortR.width
                    font.pixelSize: parent.font.pixelSize*0.8
                    color: "black"
                    validator: IntValidator{bottom: 0; top: 65535;}
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        Text {
            text: "Enter address for bind\n(" + networkSettings.ip + " on stored):"
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: title.font.pixelSize * 0.8
        }
        TextField {
            id: fieldIp
            anchors.horizontalCenter: parent.horizontalCenter
            width: fieldPortR.width
            inputMask: "000.000.000.000;_" //validator не используется здесь из-за различных имен классов в Qt5 и Qt6
            font.pixelSize: fieldPortR.parent.font.pixelSize*0.8
            color: "black"
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            text: "Enter the path to the folder with the\napplication server (optional):"
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            verticalAlignment: Text.AlignBottom
            font.pixelSize: fieldPortR.parent.font.pixelSize
        }
        TextField {
            id: fieldPath
            anchors.horizontalCenter: parent.horizontalCenter
            width: settingWinRoot.width*0.9
            placeholderText: networkSettings.pathToAppFolder
            font.pixelSize: fieldPortR.parent.font.pixelSize*0.8
            color: "black"
            horizontalAlignment: Text.AlignHCenter
        }
        CheckBox {
            checked: true
            checkState: networkSettings.visibleSettingsOnStart ? Qt.Checked : Qt.Unchecked
            text: "Show this window on start app"
            font.pixelSize: fieldPortR.parent.font.pixelSize
            anchors.horizontalCenter: parent.horizontalCenter
            onCheckStateChanged: checkState == Qt.Checked ?
                                     networkSettings.visibleSettingsOnStart = true :
                                     networkSettings.visibleSettingsOnStart = false
        }
        CheckBox {
            checked: true
            checkState: networkSettings.runNetworkOnStartWithDef ? Qt.Checked : Qt.Unchecked
            text: "Network start at application\nstartup (with stored values)"
            font.pixelSize: fieldPortR.parent.font.pixelSize * 0.8
            anchors.horizontalCenter: parent.horizontalCenter
            onCheckStateChanged: checkState == Qt.Checked ?
                                     networkSettings.runNetworkOnStartWithDef = true :
                                     networkSettings.runNetworkOnStartWithDef = false
        }
        Item {
            id: buttonsBox
            width: parent.width*0.5
            height: parent.width*0.1
            anchors.horizontalCenter: parent.horizontalCenter
            property var regIp: /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/gi

            function isEmpty(str) {
              if (str.trim() === '')
                return true;

              return false;
            }

            Button {
                text: "Apply"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                height: parent.height
                width: parent.width * 0.4
                property string _path: ""
                onClicked: {
                    var _ip, _portR, _portW

                    if(!buttonsBox.isEmpty(fieldPortR.text))
                         _portR = fieldPortR.text
                    else _portR = networkSettings.portR

                    if(!buttonsBox.isEmpty(fieldPortW.text))
                         _portW = fieldPortW.text
                    else _portW = networkSettings.portW

                    if(!buttonsBox.isEmpty(fieldPath.text))
                         _path = fieldPath.text
                    else _path = networkSettings.pathToAppFolder

                    if(buttonsBox.regIp.test(fieldIp.text)) {
                        _ip = fieldIp.text
                        warningText.visible = false
                    }
                    // @disable-check M325
                    else if(fieldIp.text !== "..."){
                        warningText.text = "Failed to apply address!"
                        warningText.visible = true
                        return
                    }
                    else _ip = networkSettings.ip
                    changeParamsNetwork(_portR, _portW, _ip);
                    pauserInBox.start()
                }

                Timer {
                    id: pauserInBox
                    interval: 100
                    running: false
                    onTriggered: changePathServer(parent._path)
                }
            }
            Button {
                text: "Remember"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                height: parent.height
                width: parent.width * 0.4
                onClicked: {
                    if(!buttonsBox.isEmpty(fieldPortR.text))
                        networkSettings.portR = fieldPortR.text
                    if(!buttonsBox.isEmpty(fieldPortW.text))
                        networkSettings.portW = fieldPortW.text
                    if(buttonsBox.regIp.test(fieldIp.text)) {
                        networkSettings.ip = fieldIp.text
                        warningText.visible = false
                    }
                    // @disable-check M325
                    else if(fieldIp.text !== "..."){
                        warningText.text = "Failed to save address"
                        warningText.visible = true
                    }
                    if(!buttonsBox.isEmpty(fieldPath.text))
                        networkSettings.pathToAppFolder = fieldPath.text
                }
            }
        }
        Text {
            id: warningText
            text: ""
            anchors.horizontalCenter: parent.horizontalCenter
            color: "red"
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: parent.height * 0.05
            visible: false
        }
    }

    function clear() {
        fieldPortW.text = ""
        fieldPortR.text = ""
        fieldPath.text = ""
        fieldIp.text = "..."
        warningText.visible = false
    }

    function showWithErrorNetwork(isNoError=false) {
        if(!isNoError) {
            visible = true
            warningText.text = "Network error. Check the data."
            warningText.visible = true
        }
    }

    onClosing: {
        clear()
    }
}
