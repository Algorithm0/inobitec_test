import QtQuick
import QtQuick.Shapes

Rectangle {
    id: jumper
    width: 3
    color: "blue"
    height: Math.sqrt(Math.pow(x_end-x_begin, 2) + Math.pow(y_end-y_begin, 2))
    x: x_begin
    y: y_begin
    property int num: -1
    property int valueBegin: 0
    property int valueEnd: 0
    property real x_begin: 0
    property real y_begin: 0
    property real x_end: 0
    property real y_end: 0
    property real angle: Math.asin(parent.widthCell/height) * 180 / Math.PI
    transform: Rotation {
        id: recRot
        origin.x: width/2
        angle: y_end < y_begin ? jumper.angle+180 : -jumper.angle
    }

    function calcX_centers(){
        x_begin = parent.calcX_center(num) - width/2;
        x_end = parent.calcX_center(num+1) - width/2;
    }

    function calcY_centers(){
        y_begin = parent.calcY_center(valueBegin); //= parent.points[num].y_center;
        y_end = parent.calcY_center(valueEnd); //= parent.points[num+1].y_center;
    }

    onValueEndChanged: calcY_centers()
    onNumChanged: calcX_centers()

    Component.onCompleted: {
        //parent.widthChanged.connect(calcX_centers);
        parent.heightChanged.connect(calcY_centers);
    }

    Component.onDestruction: {
        //parent.widthChanged.disconnect(calcX_centers);
        parent.heightChanged.disconnect(calcY_centers);
    }

    function setBeginEnd(p1, p2) {
        x_begin = p1.x_center - width/2
        y_begin = p1.y_center
        x_end = p2.x_center - width/2
        y_end = p2.y_center
        valueBegin = p1.value
        valueEnd = p2.value
        num = p1.num
    }
}
