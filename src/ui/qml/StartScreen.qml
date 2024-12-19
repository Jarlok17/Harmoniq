import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Rectangle {
    id: startScreen
    anchors.fill: parent
    color: "lightgrey"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: "Harmoniq"
            font.pointSize: 32
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            text: "Create New Image"
            onClicked: dialogNewImage.open()
        }

        Button {
            text: "Open Existing Project"
            onClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Open a Project"
        onAccepted: {
            canvasManager.loadFromHar(file);
            startScreen.visible = false;
            canvasLoader.visible = true;
        }
    }

    Dialog {
        id: dialogNewImage
        modal: true
        visible: false
        title: "New Image Settings"

        contentItem: Column {
            spacing: 10
            padding: 10

            Label { text: "Width:" }
            TextField { id: imageWidth; placeholderText: "Enter width"; text: "800" }

            Label { text: "Height:" }
            TextField { id: imageHeight; placeholderText: "Enter height"; text: "600" }

            Label { text: "Background Color:" }
            ComboBox {
                id: bgColor
                model: ["White", "Black", "Transparent"]
            }

            Row {
                spacing: 10
                Button {
                    text: "Create"
                    onClicked: {
                        canvasLoader.item.createNewCanvas(imageWidth.text, imageHeight.text, bgColor.currentText);
                        dialogNewImage.close();
                        startScreen.visible = false;
                        canvasLoader.visible = true;
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: dialogNewImage.close()
                }
            }
        }
    }
}

