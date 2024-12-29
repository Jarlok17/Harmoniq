import QtQuick 2.15
import QtQuick.Window
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

    //Start screen 
    Loader {
        id: startScreen
        source: "qrc:/UI/StartScreen.qml"
        anchors.top: menuLoader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        property bool toolsVisible: false
    }

    //Top menu 
    Loader {
        id: menuLoader
        source: "qrc:/UI/ToolBars/TopMenu.qml"
        anchors.top: parent.top
    }

    //Top toolbar
    Loader {
        id: topToolBarLoader
        source: "qrc:/UI/ToolBars/TopBar.qml"
        anchors.top: menuLoader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: startScreen.toolsVisible
    }
    
    //Canvas 
    Loader {
        id: canvasLoader
        z: -3
        source: "qrc:/UI/Canvas.qml"
        anchors.fill: parent
        anchors.topMargin: menuLoader.height + 30
        visible: startScreen.toolsVisible

        Connections {
            target: startScreen.item
            onNewImageCreated: {
                if (canvasLoader.item && canvasLoader.item.canvas) {
                    canvasLoader.item.canvas.canvasWidth = width;
                    canvasLoader.item.canvas.canvasHeight = height;
                    canvasLoader.item.canvas.backgroundColor = color;
                }
            }
}
    }
    
    //Left toolbar
    Loader {
        id: leftBarLoader
        source: "qrc:/UI/ToolBars/LeftBar.qml"
        anchors.left: parent.left
        anchors.top: topToolBarLoader.bottom
        anchors.bottom: parent.bottom
        visible: startScreen.toolsVisible
    }

    Loader {
        id: rightBarLoader
        source: "qrc:/UI/ToolBars/RightBar.qml"
        anchors.right: parent.right
        anchors.top: topToolBarLoader.bottom
        anchors.bottom: parent.bottom
    }
}
