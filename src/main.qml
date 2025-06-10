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
    Material.theme: currentTheme === Themes.darkTheme ? Material.Dark : Material.Light
    Material.primary: currentTheme.primary
    Material.accent: currentTheme.accent
    
    Rectangle {
        id: windowBackground
        width: root.width
        height: root.height
        z: -3
        color: currentTheme.background
    }

    //Start screen 
    Loader {
        id: startScreen
        source: "qrc:/UI/StartScreen.qml"
        anchors.top: menuLoader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    //Top menu 
    Loader {
        id: menuLoader
        source: "qrc:/UI/ToolBars/TopMenu.qml"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        z: 1
    }
    
    //Left toolbar
    Loader {
        id: leftBarLoader
        source: "qrc:/UI/ToolBars/LeftBar.qml"
        anchors.left: parent.left
        anchors.top: menuLoader.bottom
        anchors.bottom: parent.bottom
        visible: false 
        z: 1
    }

    //right toolbar
    Loader {
        id: rightBarLoader
        source: "qrc:/UI/ToolBars/RightBar.qml"
        anchors.right: parent.right
        anchors.top: menuLoader.bottom
        anchors.bottom: parent.bottom
        visible: false
        z: 1
    }

    // topBar
    Loader {
        id: topBarLoader
        source: "qrc:/UI/ToolBars/TopBar.qml"
        anchors.right: rightBarLoader.left
        anchors.left: leftBarLoader.right
        anchors.top: menuLoader.bottom
        visible: false
        z: 1
    }

    // TabBar loader 
    Loader {
        id: tabBarLoader
        source: "qrc:/UI/DocumentTab.qml"
        anchors.right: rightBarLoader.left
        anchors.left: leftBarLoader.right
        anchors.top: topBarLoader.bottom 
        z: 1
    }

    //layer loader 
    Loader {
        id: layerLoader
        source: "qrc:/UI/Layer.qml"
        anchors.top: tabBarLoader.bottom
        anchors.left: leftBarLoader.right
        anchors.right: rightBarLoader.left
        anchors.bottom: parent.bottom
        visible: false
    }
}
