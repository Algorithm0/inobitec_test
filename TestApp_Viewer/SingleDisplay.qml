import QtQuick
import QtQuick.Controls

Rectangle {
    id: dep
    width: parent.width
    height: parent.height/3
    color: "transparent"
    border {
        color: "green"
        width: 2
    }
    clip: true
    property string labelText: "name"
    property int min: 100
    property int max: 200
    property int maxCountPoints: 200
    property color cellColor: "#bdc3c7"
    property real desTextSize: dep.height/cells.numGridRow*0.65
    property color desTextColor: "black"
    property int numGridRow: 10
    property real graphicSize: 0

    Flickable {
        id: flickObject
        width: parent.width
        height: dep.height - dep.border.width
        contentWidth: Math.max(width, graphicSize)
        interactive: true
        boundsMovement: Flickable.StopAtBounds

        Checkered {
            id: cells
            property var points: []
            property var jumpers: []
            numGridRow: dep.numGridRow
            width: (dep.maxCountPoints + 2) * widthCell //Screen.desktopAvailableWidth
            height: parent.height
            color: cellColor
            widthLine: 1
        }

        Timer {
            id: timerFlick
            running: false; repeat: false
            onTriggered: parent.cancelFlick()
        }

        function flickTo(zn, sec){
            timerFlick.interval = sec*1000;
            flickObject.flick(zn/sec, 0);
            timerFlick.start();
        }
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: -height*0.15
        anchors.left: parent.left
        text: dep.max
        font.pixelSize: desTextSize
        font.bold: true
        verticalAlignment: Text.AlignTop
        color: desTextColor
    }

    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -height*0.15
        anchors.left: parent.left
        text: dep.min
        font.pixelSize: desTextSize
        font.bold: true
        verticalAlignment: Text.AlignBottom
        color: desTextColor
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: -height*0.15
        anchors.right: parent.right
        text: dep.max
        font.pixelSize: desTextSize
        font.bold: true
        verticalAlignment: Text.AlignTop
        color: desTextColor
    }

    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -height*0.15
        anchors.right: parent.right
        text: dep.min
        font.pixelSize: desTextSize
        font.bold: true
        verticalAlignment: Text.AlignBottom
        color: desTextColor
    }

    Text{
        id: label
        text: labelText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        color: desTextColor
        font.pixelSize: desTextColor*1.5
    }
    Rectangle {
        id: mouseValue
        color: "white"
        width: mouseValueText.font.pixelSize*2
        height: mouseValueText.font.pixelSize
        anchors.top: label.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false

        Text {
            id: mouseValueText
            text: ""
            anchors.centerIn: parent
            color: "red"
            font.pixelSize: label.font.pixelSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignTop
        }
    }

    function flickToEnd(){
        flickObject.flickTo(-graphicSize, 0.2)
    }

    function showValue(value){
        mouseValueText.text = value;
        mouseValue.visible = true;
    }

    function hideValue(){
        mouseValueText.text = "";
        mouseValue.visible = false;
    }

    function newValueReceived(newValue) {
        if(cells.points.length < maxCountPoints) {
            var component = Qt.createComponent("Point.qml");
            cells.points[cells.points.length] = component.createObject(cells);
            cells.points[cells.points.length-1].num = cells.points.length-1;
            cells.points[cells.points.length-1].value = newValue;
            cells.points[cells.points.length-1].calcX_center();
            cells.points[cells.points.length-1].calcY_center();
            graphicSize = cells.points[cells.points.length-1].x_center + 2 * cells.widthCell;

            if(cells.points.length > 1) {
                component = Qt.createComponent("Jumper.qml");
                cells.jumpers[cells.jumpers.length] = component.createObject(cells);
                cells.jumpers[cells.jumpers.length-1].setBeginEnd(cells.points[cells.points.length-2],
                                                                  cells.points[cells.points.length-1]);
            }

            if(flickObject.visibleArea.xPosition + flickObject.visibleArea.widthRatio >= 0.8
                    && !flickObject.dragging)
                flickObject.flickTo(-graphicSize, 0.1)
        }
        else {
            var tmpPoint = cells.points.shift();
            var tmpJumper = cells.jumpers.shift();
            cells.points[cells.points.length-1].num--;
            for(var i=cells.jumpers.length-1; i>=0; i--){
                cells.points[i].num--;
                cells.jumpers[i].num--;
            }
            tmpPoint.num = cells.points.length;
            tmpPoint.value = newValue;
            cells.points[cells.points.length]=tmpPoint;
            tmpJumper.setBeginEnd(cells.points[cells.points.length-2],
                                  cells.points[cells.points.length-1]);
            cells.jumpers[cells.jumpers.length]=tmpJumper;
            var vis = flickObject.visibleArea.xPosition + flickObject.visibleArea.widthRatio;
            if(vis <= 0.8 && vis >= 0.15 && !flickObject.dragging) flickObject.flickTo(cells.widthCell, 0.1);
        }
    }
}
