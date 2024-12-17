import QtQuick 2.15
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls 2.15

import Harmoniq 1.0

ApplicationWindow {
    id: root
    visible: true
    title: "Harmoniq"

    width: Screen.width
    height: Screen.height

    property var currentTheme: Themes.darkTheme
    color: currentTheme.background
    Material.theme: currentTheme === Themes.darkTheme ? Material.Dark : Material.Light
    Material.primary: currentTheme.primary
    Material.accent: currentTheme.accent

    //Top menu 
    Loader {
        id: menuLoader
        source: "qrc:/UI/ToolBars/TopMenu.qml"
        anchors.top: parent.top
    }

    Loader {
        id: topToolBarLoader
        source: "qrc:/UI/ToolBars/TopBar.qml"
        anchors.top: menuLoader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
    
    //Canvas 
    Loader {
        id: canvasLoader
        z: -3
        source: "qrc:/UI/Canvas.qml"
        anchors.fill: parent
        anchors.topMargin: menuLoader.height + 30
    }
    
    //Left toolbar
    Loader {
        id: leftBarLoader
        source: "qrc:/UI/ToolBars/LeftBar.qml"
        anchors.left: parent.left
        anchors.top: topToolBarLoader.bottom
        anchors.bottom: parent.bottom
    }
}
