import QtQuick 2.15
import QtQuick.Controls 2.15

import Harmoniq 1.0
import Harmoniq_backend 1.0

Item {
    id: colorPicker
    anchors.fill: parent

    property real hue: 0.0
    property real saturation: 1.0
    property real value: 1.0
    property real alpha: 1.0
    readonly property color selectedColor: Qt.hsva(hue, saturation, value, alpha)

    ListModel {
        id: colorHistory
    }

    Timer {
        id: colorHistoryTimer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            addColorToHistory(colorPicker.selectedColor);
        }
    }

    function addColorToHistory(color) {
        var qmlColor = Qt.color(color);
        if (qmlColor.a === 0) {
            console.log("Skipping transparent color");
            return;
        }

        for (var i = 0; i < colorHistory.count; i++) {
            var existingColor = colorHistory.get(i).color;
            if (Math.abs(existingColor.r - qmlColor.r) < 0.01 &&
                Math.abs(existingColor.g - qmlColor.g) < 0.01 &&
                Math.abs(existingColor.b - qmlColor.b) < 0.01 &&
                Math.abs(existingColor.a - qmlColor.a) < 0.01) {
                console.log("Skipping near-duplicate color:", qmlColor.toString());
                return;
            }
        }

        console.log("Adding color to history:", qmlColor.toString());
        colorHistory.insert(0, { "color": qmlColor });
        if (colorHistory.count > 10) {
            colorHistory.remove(10);
        }
    }

    Column {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        spacing: 10

        // HUE BAR
        Rectangle {
            id: hueBar
            width: parent.width
            height: 20
            radius: 4
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "red" }
                GradientStop { position: 0.17; color: "yellow" }
                GradientStop { position: 0.33; color: "green" }
                GradientStop { position: 0.5; color: "cyan" }
                GradientStop { position: 0.67; color: "blue" }
                GradientStop { position: 0.83; color: "magenta" }
                GradientStop { position: 1.0; color: "red" }
            }

            Rectangle {
                id: hueHandle
                width: 4
                height: parent.height
                x: colorPicker.hue * parent.width - width / 2
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onPositionChanged: (mouse) => {
                    colorPicker.hue = Math.max(0, Math.min(1, mouse.x / hueBar.width));
                    colorHistoryTimer.restart();
                }
                onPressed: (mouse) => {
                    colorPicker.hue = Math.max(0, Math.min(1, mouse.x / hueBar.width));
                    colorHistoryTimer.restart();
                }
                onReleased: {
                    colorHistoryTimer.restart();
                }
            }
        }

        // SATURATION/VALUE FIELD
        Rectangle {
            id: svField
            width: parent.width
            height: 200
            clip: true

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 1.0; color: Qt.hsva(colorPicker.hue, 1, 1, 1) }
                }
            }

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 1.0; color: "black" }
                }
            }

            Rectangle {
                id: selector
                width: 12
                height: 12
                radius: 6
                border.color: "white"
                border.width: 2
                color: "transparent"
                z: 2

                x: colorPicker.saturation * svField.width - width / 2
                y: (1 - colorPicker.value) * svField.height - height / 2
            }

            MouseArea {
                anchors.fill: parent
                onPositionChanged: (mouse) => {
                    colorPicker.saturation = Math.max(0, Math.min(1, mouse.x / svField.width));
                    colorPicker.value = Math.max(0, Math.min(1, 1 - (mouse.y / svField.height)));
                    colorHistoryTimer.restart(); // Restart timer on SV change
                }
                onPressed: (mouse) => {
                    colorPicker.saturation = Math.max(0, Math.min(1, mouse.x / svField.width));
                    colorPicker.value = Math.max(0, Math.min(1, 1 - (mouse.y / svField.height)));
                    colorHistoryTimer.restart();
                }
                onReleased: {
                    colorHistoryTimer.restart();
                }
            }
        }

        // ALPHA BAR
        Rectangle {
            id: alphaBar
            width: parent.width
            height: 20
            radius: 4
            clip: true

            Repeater {
                model: 20
                delegate: Rectangle {
                    width: alphaBar.width / 20
                    height: alphaBar.height
                    color: (index % 2 === 0) ? "#ccc" : "#eee"
                    x: index * width
                }
            }

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 1.0; color: Qt.hsva(colorPicker.hue, colorPicker.saturation, colorPicker.value, 1.0) }
                    GradientStop { position: 0.0; color: Qt.hsva(colorPicker.hue, colorPicker.saturation, colorPicker.value, 0.0) }
                }
            }

            Rectangle {
                id: alphaHandle
                width: 4
                height: parent.height
                x: colorPicker.alpha * parent.width - width / 2
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onPositionChanged: (mouse) => {
                    colorPicker.alpha = Math.max(0, Math.min(1, mouse.x / alphaBar.width));
                    colorHistoryTimer.restart(); // Restart timer on alpha change
                }
                onPressed: (mouse) => {
                    colorPicker.alpha = Math.max(0, Math.min(1, mouse.x / hueBar.width));
                    colorHistoryTimer.restart();
                }
                onReleased: {
                    colorHistoryTimer.restart();
                }
            }
        }

        // COLOR PREVIEW
        Rectangle {
            width: parent.width
            height: 30
            color: colorPicker.selectedColor
            border.color: "white"

            Text {
                anchors.centerIn: parent
                color: "white"
                text: colorPicker.selectedColor
                font.pixelSize: 14
            }
        }

        // COLOR SWATCHES
        Grid {
            width: parent.width
            columns: 5
            rows: 2
            spacing: 5

            Repeater {
                model: colorHistory
                delegate: Rectangle {
                    width: (parent.width - (parent.columns - 1) * parent.spacing) / parent.columns
                    height: 24
                    color: model.color
                    border.color: "white"
                    border.width: 1

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            ToolSettings.setColor(model.color);
                        }
                    }
                }
            }
        }
    }

    onSelectedColorChanged: {
        colorHistoryTimer.restart();
    }
}
