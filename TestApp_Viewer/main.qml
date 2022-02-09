import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    id: root
    width: 640
    minimumWidth: 190
    height: 480
    minimumHeight: 220
    visible: true
    title: qsTr("Tonometer")

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
        source: "qrc:/play_icon.png"
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

    signal startServer()

    function newData(hp, lp, pl) {
        dep1.newValueReceived(hp);
        dep2.newValueReceived(lp);
        dep3.newValueReceived(pl);
        bar.setValues(hp, lp, pl);
    }
}
