import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Harmoniq 1.0

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
        }
        MenuItem {
            text: qsTr("Save...")
            font.pixelSize: Themes.currentTheme.fontSize
        }
        MenuItem {
            text: qsTr("Save as...")
            font.pixelSize: Themes.currentTheme.fontSize
            onTriggered: saveDialog.open()
        }
    }

    Menu {
        title: qsTr("Edit")
        MenuItem {
            text: qsTr("Undo")
            font.pixelSize: Themes.currentTheme.fontSize
        }
        MenuItem {
            text: qsTr("Redo")
            font.pixelSize: Themes.currentTheme.fontSize
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
        title: qsTr("Save Image As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["Images (*.png *.jpg *.bmp)", "All Files (*)"]
        onAccepted: {
            console.log("Selected file:", selectedFile)
            layerManager.mergeLayers(selectedFile)
        }
    }
}


