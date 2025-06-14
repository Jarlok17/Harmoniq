import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Window
import Harmoniq 1.0
import Harmoniq_backend 1.0

MenuBar {
    id: menuBar
    
    property int canvasWidth: 800
    property int canvasHeight: 600
    property string canvasBackgroundColor: "White"

    Menu {
        font.pixelSize: Themes.currentTheme.fontSize
        title: qsTr("File")
        MenuItem {
            text: qsTr("New...")
            onTriggered: dialogNewImage.open()
        }
        MenuItem {
            text: qsTr("Open...")
            onTriggered: openDialog.open()
        }
        Action {
            text: qsTr("Save...")
            enabled: documentManager && documentManager.current ? true : false
            shortcut: "Ctrl+S"
            onTriggered: {
                const currentPath = documentManager && documentManager.current ? documentManager.current.path : "";
                console.log("Save triggered with path:", currentPath);
                if (!currentPath || currentPath === "") {
                    saveDialog.open();
                } else {
                    const success = documentManager.saveToFile(currentPath);
                    if (documentManager && documentManager.current) {
                        if (!success) {
                            console.warn("Save failed, modified state unchanged");
                        } else {
                            documentManager.current.setModified(false);
                            console.log("Saved successfully to:", currentPath);
                        }
                    }
                }
            }
        }
        MenuItem {
            text: qsTr("Save as...")
            enabled: documentManager && documentManager.current ? true : false
            onTriggered: saveDialog.open()
        }
        MenuItem {
            text: qsTr("Export as image...")
            enabled: documentManager && documentManager.current ? true : false
            onTriggered: exportDialog.open()
        }
    }

    Connections {
        target: documentManager && documentManager.current ? documentManager.current.historyManager : null
        function onStateChanged() {
            undoAction.enabled = documentManager.current.historyManager.canUndo();
            redoAction.enabled = documentManager.current.historyManager.canRedo();
        }
    }

    Menu {
        title: qsTr("Edit")
        font.pixelSize: Themes.currentTheme.fontSize
        Action {
            id: undoAction
            text: qsTr("Undo")
            shortcut: "Ctrl+Z"
            enabled: documentManager && documentManager.current ? documentManager.current.historyManager.canUndo() : false
            onTriggered: {
                console.log("Undo triggered. Can undo:", documentManager.current.historyManager.canUndo());
                documentManager.current.historyManager.undo();
            }
        }
        Action {
            id: redoAction
            text: qsTr("Redo")
            shortcut: "Ctrl+Shift+Z"
            enabled: documentManager && documentManager.current ? documentManager.current.historyManager.canRedo() : false
            onTriggered: {
                console.log("Redo triggered. Can redo:", documentManager.current.historyManager.canRedo());
                documentManager.current.historyManager.redo();
            }
        }
    }

    Menu {
        title: qsTr("Panels")
        MenuItem {
            text: qsTr("Toggle Left Toolbar")
            font.pixelSize: Themes.currentTheme.fontSize
            enabled: documentManager && documentManager.current ? true : false
            onTriggered: {
                leftBarLoader.visible = !leftBarLoader.visible
                console.log("Left Toolbar visibility:", leftBarLoader.visible)
            }
        }
        MenuItem {
            text: qsTr("Toggle Right Toolbar")
            font.pixelSize: Themes.currentTheme.fontSize
            enabled: documentManager && documentManager.current ? true : false
            onTriggered: {
                rightBarLoader.visible = !rightBarLoader.visible
                console.log("Right Toolbar visibility:", rightBarLoader.visible)
            }
        }
        MenuItem {
            text: qsTr("Toggle Top Panel")
            font.pixelSize: Themes.currentTheme.fontSize
            enabled: documentManager && documentManager.current ? true : false
            onTriggered: {
                topBarLoader.visible = !tpoBarLoader.visible
                console.log("Layers Panel visibility:", layerLoader.visible)
            }
        }
    }

    Menu {
        title: qsTr("Help")
        MenuItem {
            text: qsTr("About")
            font.pixelSize: Themes.currentTheme.fontSize
            onTriggered: aboutWindow.visible = true
        }
    }
    
    Window {
        id: aboutWindow
        title: qsTr("Про Harmoniq")
        maximumWidth: 500
        maximumHeight: 200
        minimumWidth: 500 
        minimumHeight: 200
        visible: false
        color: Themes.currentTheme.background
        modality: Qt.ApplicationModal
        flags: Qt.Dialog

        x: (Screen.width - width) / 2
        y: (Screen.height - height) / 2

        Column {
            anchors.centerIn: parent
            spacing: 10
            Text {
                text: qsTr("Harmoniq - кросплатформний графічний застосунок\nВерсія: 1.0\nАвтор: Ярослав\nЛіцензія: MIT")
                font.pixelSize: Themes.currentTheme.fontSize
                color: "white"
            }
            Text {
                text: qsTr("© 2025 Harmoniq. Усі права захищено.")
                font.pixelSize: Themes.currentTheme.fontSize - 2
                color: "white"
            }
            Button {
                text: qsTr("OK")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: aboutWindow.visible = false
            }
        }
    }
    FileDialog {
        id: saveDialog
        title: qsTr("Save Project As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["Harmoniq Projects (*.json)", "All Files (*)"]
        onAccepted: {
            if (selectedFile && selectedFile !== "") {
                var fullPath = selectedFile.toString();
                var localPath = fullPath.replace("file://", "");
                var fileName = localPath.split('/').pop().split('.json')[0];
                if (documentManager && documentManager.current) {
                    documentManager.current.setName(fileName);
                    documentManager.current.setPath(localPath);
                    documentManager.saveToFile(selectedFile);
                    documentManager.current.setModified(false);
                }
            }
        }
    }

    FileDialog {
        id: openDialog
        title: qsTr("Open Project")
        fileMode: FileDialog.OpenFile
        nameFilters: ["Harmoniq Projects (*.json)"]
        onAccepted: {
            console.log("OPEN DIALOG ACCEPTED:", selectedFile)

            startScreen.visible = false;
            leftBarLoader.visible = true;
            rightBarLoader.visible = true;
            topBarLoader.visible = true;
            layerLoader.visible = true; 

            documentManager.loadFromFile(selectedFile)
        }
    }

    FileDialog {
        id: exportDialog
        title: qsTr("Export Image As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG (*.png)", "JPEG (*.jpg *.jpeg)", "All Files (*)"]
        onAccepted: {
            if (selectedFile) {
                console.log("Exporting to:", selectedFile);
                documentManager.exportImage(selectedFile.toString());
            }
        }   
    }

    Popup {
        id: dialogNewImage
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        anchors.centerIn: Overlay.overlay

        background: Rectangle {
            color: Themes.currentTheme.background
            radius: 10
        }

        contentItem: Column {
            spacing: 10
            padding: 10

            Label {
                text: qsTr("Width:")
            }

            TextField {
                id: widthField
                placeholderText: qsTr("Enter width")
                text: "800"
            }

            Label {
                text: qsTr("Height:")
            }

            TextField {
                id: heightField
                placeholderText: qsTr("Enter height")
                text: "600"
            }

            Label {
                text: qsTr("Background Color:")
            }

            Row {
                spacing: 10
                Button {
                    id: selectColor
                    text: qsTr("Select color")
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
                    text: qsTr("Create")
                    onClicked: {
                        if (parseInt(widthField.text) > 0 && parseInt(heightField.text) > 0) {
                            canvasWidth = parseInt(widthField.text);
                            canvasHeight = parseInt(heightField.text);
                            canvasBackgroundColor = selectedColor.color;
                            
                            startScreen.visible = false;
                            leftBarLoader.visible = true;
                            rightBarLoader.visible = true;
                            topBarLoader.visible = true;
                            layerLoader.visible = true;

                            documentManager.addDocument("Untitled", "");
                            documentManager.setCurrentIndex(documentManager.count() - 1);
                            documentManager.currentLayerManager.addLayer("background", canvasWidth, canvasHeight, canvasBackgroundColor, false);
                            
                            dialogNewImage.close();
                        } else {
                            console.log("Invalid dimensions");
                        }
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    onClicked: dialogNewImage.close()
                }
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: qsTr("Select the color...")
        onAccepted: {
            selectedColor.color = colorDialog.selectedColor;
            canvasBackgroundColor = colorDialog.selectedColor;
        }
    }
}
