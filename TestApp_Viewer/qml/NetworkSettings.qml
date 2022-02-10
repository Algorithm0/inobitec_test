import QtQuick 2.12
import Qt.labs.settings 1.0

Settings {
    id: colorThemes
    category: "network"
    property bool visibleSettingsOnStart: true
    property bool runNetworkOnStartWithDef: false
    property int portR: 20108
    property int portW: 8234
    property string ip: "127.0.0.1"
    property string pathToAppFolder: "."
}
