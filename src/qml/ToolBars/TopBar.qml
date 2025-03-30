import QtQuick 2.15
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls 2.15

import Harmoniq 1.0

Item {
    id: topBar
    width: parent.width 
    height: 30
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    Rectangle {
        id: menuBackground
        color: currentTheme.background
        anchors.fill: parent
        border.color: Qt.darker(Themes.currentTheme.background, 0.5)
    }
}
