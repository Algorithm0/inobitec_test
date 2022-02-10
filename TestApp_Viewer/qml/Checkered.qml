import QtQuick 2.12

Item {
    id: rootCheckered
    property color color: "black"
    property int numGridRow: 80
    property int numGridColStatic: width/widthCell+1
    property real widthLine: 1
    property real widthCell: 19
    clip: true


    Column {
        anchors.fill: parent
        Repeater {
            model: rootCheckered.numGridRow
            Rectangle {
                width: rootCheckered.width
                height: rootCheckered.height / rootCheckered.numGridRow
                color: "transparent"
                border {
                    color: rootCheckered.color
                    width: rootCheckered.widthLine
                }
            }
        }
    }

    Row {
        id: columnsParent
        anchors.fill: parent
        property real widthRec: rootCheckered.widthCell
        property real heightRec: rootCheckered.height
        property color borderColorRec: rootCheckered.color
        property real borderWidthRec: rootCheckered.widthLine
        Repeater {
            id: columnsParentRepeater
            model: rootCheckered.numGridColStatic
            Rectangle {
                width: widthCell
                objectName: "staticColumn" + index
                height: rootCheckered.height
                color: "transparent"
                border {
                    color: rootCheckered.color
                    width: rootCheckered.widthLine
                }
            }
        }
    }

    function calcX_center(num){
        return widthCell*(num+1);
    }

    function calcY_center(value){
        return height - ((value - dep.min)*height/(dep.max-dep.min));
    }

//    function addNewColumns(count=1) {
//        var component = Qt.createComponent("ColumnInCells.qml");
//        for(var i=0; i<count; i++)
//            var newObject = component.createObject(columnsParent);
//    }
}
