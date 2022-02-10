import QtQuick 2.12

Rectangle {
    id: pointRoot
    width: 7
    height: width
    radius: width/2
    color: "red"
    property real x_center: 0
    property real y_center: 0
    property int num: 0
    property int value: 0
    x: x_center - radius
    y: y_center - radius
    z: 1

    function calcX_center(){
        x_center = parent.calcX_center(num)
    }

    function calcY_center(){
        y_center = parent.calcY_center(value)
    }

    onValueChanged: calcY_center()
    onNumChanged: calcX_center()

    Component.onCompleted: {
        //parent.widthChanged.connect(calcX_center);
        parent.heightChanged.connect(calcY_center);
    }

    Component.onDestruction: {
        //parent.widthChanged.disconnect(calcX_center);
        parent.heightChanged.disconnect(calcY_center);
    }

    MouseArea {
        width: parent.width*1.2
        height: width
        anchors.centerIn: parent
        hoverEnabled: true
        onEntered: dep.showValue(value)
        onExited: dep.hideValue()
    }
}
