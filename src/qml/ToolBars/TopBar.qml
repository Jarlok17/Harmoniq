import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Harmoniq 1.0
import Harmoniq_backend 1.0

Item {
    id: topBar
    width: parent.width
    height: 30
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }

    Rectangle {
        id: menuBackground
        color: currentTheme.background
        anchors.fill: parent
        border.color: Qt.darker(Themes.currentTheme.background, 0.5)

        RowLayout {
            id: topBarLayout
            anchors.fill: parent
            anchors.margins: 0
            spacing: 8

            // Блок для "Size" і Slider
            RowLayout {
                spacing: 4
                Layout.alignment: Qt.AlignVCenter

                Label {
                    text: "Size:"
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.leftMargin: 6
                }

                Slider {
                    id: sizeSlider
                    from: 1
                    to: 100
                    value: ToolSettings.size
                    onMoved: ToolSettings.size = value
                    Layout.fillWidth: true
                    Layout.preferredWidth: 120
                    Layout.maximumWidth: 200
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignVCenter

                    background: Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: parent.availableWidth
                        height: 10
                        color: "gray"
                        radius: 2
                    }

                    handle: Rectangle {
                        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                        y: parent.topPadding + parent.availableHeight / 2 - height / 2
                        implicitWidth: 12
                        implicitHeight: 12
                        radius: 6
                        color: parent.pressed ? "lightgray" : "white"
                        border.color: "gray"
                    }
                }
            }

            RowLayout {
                spacing: 4
                Layout.alignment: Qt.AlignVCenter

                Label {
                    text: "Fill:"
                    color: "white"
                    font.pixelSize: 12
                    font.bold: true
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                }

                CheckBox {
                    checked: ToolSettings.fill
                    onToggled: ToolSettings.fill = checked
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 30
                    Layout.alignment: Qt.AlignVCenter

                    indicator: Rectangle {
                        anchors.centerIn: parent
                        implicitWidth: 16
                        implicitHeight: 16
                        radius: 3
                        border.color: "gray"
                        border.width: 1
                        color: parent.checked ? Themes.currentTheme.accent : "transparent"

                        Rectangle {
                            anchors.centerIn: parent
                            width: 10
                            height: 10
                            color: "transparent"
                            border.color: "white"
                            border.width: 1
                            radius: 2
                            visible: !parent.parent.checked
                        }
                    }
                }
            }
        }
    }
}
