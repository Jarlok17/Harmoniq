import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import Harmoniq 1.0
import harmoniq 1.0

Item {
    id: leftMenu
    width: Screen.width * 0.05
    height: parent.height 

    property list<ToolButton> buttonManager: []

    Rectangle {
        id: menuBackground
        color: Themes.currentTheme.background
        anchors.fill: parent
        border.color: Qt.darker(Themes.currentTheme.background, 0.5)

        ColumnLayout {
            id: buttonContainer
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 5
            
            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        {icon: "qrc:/Icons/64x64/brush.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Brush); console.log("Brush PNG"); }},
                        {icon: "qrc:/Icons/64x64/erase.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Eraser); console.log("PEN PNG"); }}
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData.icon
                        buttonManager: leftMenu.buttonManager
                        onClickedAction: modelData.action
                    }
                }
            }

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        {icon: "qrc:/Icons/64x64/color-fill.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Fill); console.log("Fill PNG"); }},
                        {icon: "qrc:/Icons/64x64/color-picker.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Picker); console.log("Color-Picker PNG"); }}
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData.icon
                        buttonManager: leftMenu.buttonManager
                        onClickedAction: modelData.action
                    }            
                }
            }

            Rectangle {
                height: 1
                width: parent.width * 0.9
                color: Qt.lighter(Themes.currentTheme.background, 1.5)
                Layout.alignment: Qt.AlignHCenter
            }

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        {icon: "qrc:/Icons/64x64/move-arrows.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Move); console.log("Move Arrows PNG"); }},
                        {icon: "qrc:/Icons/64x64/shape-tool.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Shape); console.log("Shape-Tool PNG"); }}
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData.icon
                        buttonManager: leftMenu.buttonManager
                        onClickedAction: modelData.action
                    }
                }
            }

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        {icon: "qrc:/Icons/64x64/line-tool.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Line); console.log("Line Tool PNG"); }},
                        {icon: "qrc:/Icons/64x64/selection-tool.png", action: function() { documentManager.currentLayerManager.setCurrentTool(ToolType.Selection); console.log("Selection Tool PNG"); }}
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData.icon
                        buttonManager: leftMenu.buttonManager
                        onClickedAction: modelData.action
                    }
                }
            }
        }
    }
}
