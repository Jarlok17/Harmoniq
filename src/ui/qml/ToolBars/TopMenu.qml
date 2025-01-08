import QtQuick
import QtQuick.Controls
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
}

