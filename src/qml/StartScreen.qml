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

    Column {
        id: contentColumn
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.8

        Text {
            text: "Harmoniq"
            font.pixelSize: Math.min(parent.width, parent.height) * 0.2
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            text: "Create New Project"
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
        fileMode: FileDialog.OpenFile
        nameFilters: ["Harmoniq Projects (*.json)"]
        onAccepted: {
            console.log("OPEN DIALOG ACCEPTED:", selectedFile)
            documentManager.loadFromFile(selectedFile)

            startScreen.visible = false
            topBarLoader.visible = true
            leftBarLoader.visible = true
            rightBarLoader.visible = true
            layerLoader.visible = true
        }
    }

    ColorDialog {
        id: colorDialog 
        title: "Select the color..."
        onAccepted: {
            selectedColor.color = colorDialog.selectedColor;
            canvasBackgroundColor = colorDialog.selectedColor;
        }
    }

    Popup {
        id: dialogNewImage
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        visible: false
        anchors.centerIn: Overlay.overlay

        background: Rectangle {
            color: Themes.currentTheme.background
            radius: 10
        }

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

            Row {
                spacing: 10
                Button {
                    id: selectColor 
                    text: "select color"
                    onClicked: {
                        colorDialog.open()
                    }
                } 
                Rectangle {
                    id: selectedColor
                    width: 50
                    height: 50
                    color: canvasBackgroundColor
                }
            }            

            Row {
                spacing: 10
                Button {
                    text: "Create"
                    onClicked: {
                        if (parseInt(widthField.text) > 0 && parseInt(heightField.text) > 0) {
                            canvasWidth = parseInt(widthField.text);
                            canvasHeight = parseInt(heightField.text);

                            dialogNewImage.close();
                            startScreen.visible = false;
                            leftBarLoader.visible = true;
                            rightBarLoader.visible = true;
                            topBarLoader.visible = true;
                            layerLoader.visible = true;
                            
                            documentManager.addDocument("Untitled", "");
                            documentManager.currentLayerManager.addLayer("background", canvasWidth, canvasHeight, canvasBackgroundColor, false);
                        } else {
                            console.log("Invalid dimensions");
                        }
                        dialogNewImage.visible = false;
                        dialogNewImage.close();
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
        
        function onWidthChanged() {
           contentColumn.width = Math.min(parent.width * 0.8, 400) 
        }
    }
}
