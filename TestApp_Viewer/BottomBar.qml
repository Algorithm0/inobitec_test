import QtQuick

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

    function setValues(_hp, _lp, _pl) {
        hp = _hp
        lp = _lp
        pl = _pl
    }
}
