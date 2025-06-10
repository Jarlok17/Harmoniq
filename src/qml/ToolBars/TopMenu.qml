import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Harmoniq 1.0
import Harmoniq_backend 1.0

MenuBar {
    id: menuBar

    Menu {
        title: qsTr("File")
        MenuItem {
            text: qsTr("New...")
            font.pixelSize: Themes.currentTheme.fontSize
        }
        MenuItem {
            text: qsTr("Open...")
            font.pixelSize: Themes.currentTheme.fontSize
            onTriggered: openDialog.open()
        }
        Action {
            text: qsTr("Save...")
            shortcut: "Ctrl+S"
            onTriggered: {
                const currentPath = documentManager.current ? documentManager.current.path : "";
                console.log("Save triggered with path:", currentPath);
                if (!currentPath || currentPath === "") {
                    saveDialog.open();
                } else {
                    const success = documentManager.saveToFile(currentPath);
                    if (documentManager.current) {
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
            font.pixelSize: Themes.currentTheme.fontSize
            onTriggered: saveDialog.open()
        }
        MenuItem {
            text: qsTr("Export as image...")
            font.pixelSize: Themes.currentTheme.fontSize
            onTriggered: exportDialog.open()
        }
    }

    Connections {
        target: documentManager.current ? documentManager.current.historyManager : null
        function onStateChanged() {
            undoAction.enabled = documentManager.current.historyManager.canUndo();
            redoAction.enabled = documentManager.current.historyManager.canRedo();
        }
    }

    Menu {
        title: qsTr("Edit")
        Action {
            id: undoAction
            text: qsTr("Undo")
            shortcut: "Ctrl+Z"
            enabled: documentManager.current ? documentManager.current.historyManager.canUndo() : false
            onTriggered: {
                console.log("Undo triggered. Can undo:", documentManager.current.historyManager.canUndo());
                documentManager.current.historyManager.undo();
            }
        }
        Action {
            id: redoAction
            text: qsTr("Redo")
            shortcut: "Ctrl+Shift+Z"
            enabled: documentManager.current ? documentManager.current.historyManager.canRedo() : false
            onTriggered: {
                console.log("Redo triggered. Can redo:", documentManager.current.historyManager.canRedo());
                documentManager.current.historyManager.redo();
            }
        }
    }

    Menu {
        title: qsTr("View")
        MenuItem {
            text: qsTr("Zoom in")
            font.pixelSize: 14
        }
        MenuItem {
            text: qsTr("Zoom out")
            font.pixelSize: 14
        }
    }

    Menu {
        title: qsTr("Help")
        MenuItem {
            text: qsTr("About")
            font.pixelSize: Themes.currentTheme.fontSize
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
                if (documentManager.current) {
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
            documentManager.loadFromFile(selectedFile)
        }
    }

    FileDialog {
        id: exportDialog
        title: qsTr("Export Image As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG (*.png)", "JPEG (*.jpg *.jpeg)", "All Files (*)"]
        onAccepted: {
            if (selectedFile && selectedFile !== "") {
                var fullPath = selectedFile.toString();
                var localPath = fullPath.replace("file://", "");
                documentManager.exportImage(localPath);
            }
        }
    }
}
