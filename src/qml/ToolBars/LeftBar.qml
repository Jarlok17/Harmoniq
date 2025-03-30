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
                        {icon: "qrc:/Icons/64x64/brush.png", action: function() { layerManager.setCurrentTool(ToolType.Brush); console.log("Brush PNG"); }},
                        {icon: "qrc:/Icons/64x64/edit-pen.svg", action: function() { layerManager.setCurrentTool(ToolType.Eraser); console.log("PEN PNG"); }}
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
                        "qrc:/Icons/64x64/color-fill.png",
                        "qrc:/Icons/64x64/color-picker.png"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
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
                        "qrc:/Icons/64x64/move-arrows.png",
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.40
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }
                }
            }
        }
    }
}
