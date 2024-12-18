import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

import Harmoniq 1.0

Item {
    id: leftMenu
    width: (Qt.platform.os === "android" || Qt.platform.os === "ios") ? Screen.width * 0.1 : Screen.width * 0.05
    height: parent.height 

    property list<ToolButton> buttonManager: []

    Rectangle {
        id: menuBackground
        color: currentTheme.background
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
                        "qrc:/Icons/64x64/brush.svg",
                        "qrc:/Icons/64x64/edit-pen.svg"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.45
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }
                }
            }

            Rectangle {
                height: 1
                width: parent.width * 0.9
                color: Qt.lighter(currentTheme.background, 1.5)
                Layout.alignment: Qt.AlignHCenter
            }

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        "qrc:/Icons/64x64/color-fill.svg",
                        "qrc:/Icons/64x64/dropper.svg"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.45
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }            
                }
            }

            Rectangle {
                height: 1
                width: parent.width * 0.9
                color: Qt.lighter(currentTheme.background, 1.5)
                Layout.alignment: Qt.AlignHCenter
            }

            GridLayout {
                columns: 2
                Repeater {
                    model: [
                        "qrc:/Icons/64x64/move-arrows.svg",
                        "qrc:/Icons/64x64/eraser.svg"
                    ]

                    delegate: ToolButton {
                        width: leftMenu.width * 0.45
                        iconSource: modelData
                        buttonManager: leftMenu.buttonManager
                    }
                }
            }
        }
    }
}
