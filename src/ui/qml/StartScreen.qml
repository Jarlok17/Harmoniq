import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

import Harmoniq 1.0

Rectangle {
    id: startScreen
    anchors.fill: parent
    color: Themes.currentTheme.background

    property int canvasWidth: 800
    property int canvasHeight: 600
    property string canvasBackgroundColor: "White"

    signal newImageCreated(int width, int height, string color)

    Column {
        id: contentColumn
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.8

        // Заголовок
        Text {
            text: "Harmoniq"
            font.pixelSize: Math.min(parent.width, parent.height) * 0.2
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        // Кнопки
        Button {
            text: "Create New Image"
            font.pixelSize: Math.min(parent.width, parent.height) * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle {
                color: "#5cdb95"
                radius: 10
                implicitWidth: 250
                implicitHeight: 50
            }
            onClicked: dialogNewImage.open()
        }

        Button {
            text: "Open Existing Project"
            font.pixelSize: Math.min(parent.width, parent.height) * 0.05
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle {
                color: "#05386b"
                radius: 10
                implicitWidth: 250
                implicitHeight: 50
            }
            onClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Open a Project"
        onAccepted: {
            startScreen.visible = false;
            canvasLoader.visible = true;
        }
    }

    Dialog {
        id: dialogNewImage
        modal: true
        visible: false
        title: "New Image Settings"
        anchors.centerIn: parent

        background: Rectangle {
            color: Themes.currentTheme.background
            radius: 10
        }

        property alias imageWidth: widthField.text
        property alias imageHeight: heightField.text
        property alias selectedBgColor: bgColor.currentText

        contentItem: Column {
            spacing: 10
            padding: 10

            Label {
                text: "Width:"
            }

            TextField {
                id: widthField
                placeholderText: "Enter width"
                text: "800"
            }

            Label {
                text: "Height:"
            }

            TextField {
                id: heightField
                placeholderText: "Enter height"
                text: "600"
            }

            Label {
                text: "Background Color:"
            }

            ComboBox {
                id: bgColor
                model: ["White", "Black", "Transparent"]
            }

            Row {
                spacing: 10
                Button {
                    text: "Create"
                    onClicked: {
                        if (parseInt(widthField.text) > 0 && parseInt(heightField.text) > 0) {
                            canvasWidth = parseInt(widthField.text);
                            canvasHeight = parseInt(heightField.text);
                            canvasBackgroundColor = bgColor.currentText;

                            dialogNewImage.close();
                            startScreen.visible = false;
                            topToolBarLoader.visible = true;
                            leftBarLoader.visible = true;
                            rightBarLoader.visible = true;

                            console.log("Creating new image with dimensions:", canvasWidth, canvasHeight, "and color:", canvasBackgroundColor);
                            startScreen.newImageCreated(canvasWidth, canvasHeight, canvasBackgroundColor);
                        } else {
                            console.log("Invalid dimensions");
                        }
                        dialogNewImage.visible = false;
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: dialogNewImage.close()
                }
            }
        }
    }

    Component.onCompleted: {
        contentColumn.width = Math.min(parent.width * 0.8, 400)
    }

    Connections {
        target: parent
        onWidthChanged: contentColumn.width = Math.min(parent.width * 0.8, 400)
    }
}
