import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: root
    width: 640
    minimumWidth: 228
    height: 480
    minimumHeight: 220
    visible: true

    NetworkSettings {
        id: networkSettings
    }

    SingleDisplay {
        id: dep1
        labelText: "systolic pressure"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: (parent.height-bar.height)/3
    }
    SingleDisplay {
        id: dep2
        labelText: "diastolic pressure"
        anchors.top: dep1.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        min: 70
        max: 110
        numGridRow: 8
    }
    SingleDisplay {
        id: dep3
        labelText: "pulse"
        anchors.top: dep2.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        min: 40
        max: 200
        numGridRow: 8
    }
    BottomBar {
        id: bar
    }

    Image {
        id: startServerButtion
        objectName: "startServerButtion"
        source: "qrc:/img/play_icon.png"
        anchors.centerIn: parent
        width: height
        height: Math.min(parent.height, parent.width) * 0.8
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.startServer()
            }
        }
    }

    WindowNetworkSettings {
        id: winSettings
        objectName: "winSettings"
        //transientParent: root
    }

    signal startServer()

    signal changeParamsNetwork(int portR, int portW, string ip)
    signal changePathServer(string path)

    function newData(hp, lp, pl) {
        dep1.newValueReceived(hp);
        dep2.newValueReceived(lp);
        dep3.newValueReceived(pl);
        bar.setValues(hp, lp, pl);
    }

    Timer {
        id: pauser
        interval: 100
        running: false
        onTriggered: {
            changeParamsNetwork(networkSettings.portR, networkSettings.portW, networkSettings.ip);
            changePathServer(networkSettings.pathToAppFolder)
        }
    }

    Component.onCompleted: {
        if(networkSettings.visibleSettingsOnStart)
            winSettings.visible = true
        else winSettings.visible = false
        if(networkSettings.runNetworkOnStartWithDef)
            pauser.start()
    }
}
