import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import Harmoniq 1.0

Item {
    id: leftMenu
    width: (Qt.platform.os === "android" || Qt.platform.os === "ios") ? Screen.width * 0.1 : Screen.width * 0.07
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    property list<ToolButton> buttonManager: []

    Rectangle {
        id: menuBackground
        color: currentTheme.background
        anchors.fill: parent
        border.color: Qt.darker(Themes.currentTheme.background, 0.5)

        Column {
            id: buttonContainer
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        "qrc:/Icons/64x64/brush.svg",
                        "qrc:/Icons/64x64/edit-pen.svg"
                    ]

                    
                    delegate: ToolButton {
                        width: leftMenu.width * 0.35
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }
                }
            }

            Rectangle {
                height: 1
                width: parent.width * 0.9
                color: Qt.darker(Themes.currentTheme.background, 0.8)
                anchors.horizontalCenter: parent.horizontalCenter
            }

            GridLayout {
                columns: 2
                columnSpacing: 4
                rowSpacing: 4
                Repeater {
                    model: [
                        "qrc:/Icons/64x64/color-fill.svg",
                        "qrc:/Icons/64x64/dropper.svg"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.35
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }            
                }
            }

            Rectangle {
                height: 1
                width: parent.width * 0.9
                color: Qt.darker(Themes.currentTheme.background, 0.8)
                anchors.horizontalCenter: parent.horizontalCenter
            }

            GridLayout {
                columns: 2
                columnSpacing: 4
                rowSpacing: 4
                Repeater {
                    model: [
                        "qrc:/Icons/64x64/move-arrows.svg",
                        "qrc:/Icons/64x64/eraser.svg"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.35
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }
                }
            }
        }
    }
}
