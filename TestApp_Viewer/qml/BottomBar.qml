import QtQuick 2.12

Item {
    width: parent.width
    height: parent.height*0.2
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    property int hp: 0
    property int lp: 0
    property int pl: 0

    Text {
        id: textCurrentValues
        text: "Current: " + hp + '/' + lp + " ♡" + pl
        color: "red"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: parent.height*0.18

        MouseArea {
            anchors.fill: parent
            onClicked: {
                dep1.flickToEnd()
                dep2.flickToEnd()
                dep3.flickToEnd()
            }
        }
    }
    Image {
        id: showWinSettingButtion
        source: "qrc:/img/settings-icon.png"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: height
        height: textCurrentValues.height
        visible: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                winSettings.visible = true
            }
        }
    }

    function setValues(_hp, _lp, _pl) {
        hp = _hp
        lp = _lp
        pl = _pl
    }
}
