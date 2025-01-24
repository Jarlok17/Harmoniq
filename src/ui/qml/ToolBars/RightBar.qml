import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Dialogs

import Harmoniq 1.0
import Harmoniq_backend 1.0

Rectangle {
    id: rightBar
    width: (Qt.platform.os === "android" || Qt.platform.os === "ios") ? Screen.width * 0.1 : Screen.width * 0.16
    height: parent.height
    color: Themes.currentTheme.background
    border.color: Qt.darker(Themes.currentTheme.background, 0.5)

    property bool isVisible: false
    property real hiddenX: parent.width
    property real visibleX: parent.width - width

    property int layerWidth: 800 
    property int layerHeight: 600

    property string currentMode: "layers"

    x: isVisible ? visibleX : hiddenX

    
    Rectangle {
        id: colorContainer
        width: rightBar.width
        height: 0
        color: Themes.currentTheme.background
        border.color: Themes.currentTheme.accent
        anchors.top: rightBar.top

        
    }

    Rectangle {
        id: bottomContainer
        width: rightBar.width
        height: rightBar.height / 2
        color: Themes.currentTheme.background
        border.color: Themes.currentTheme.primary
        anchors.bottom: rightBar.bottom
        
        
        Rectangle {
            id: layerContainerTopMenu
            width: bottomContainer.width
            height: 40
            color: Themes.currentTheme.background
            border.color: Qt.darker(Themes.currentTheme.background, 0.5)
            anchors.top: bottomContainer.top
            visible: rightBar.currentMode == "layers"
        }
        

        Rectangle {
            id: layerContainerBottomMenu
            width: bottomContainer.width
            height: 50
            color: Themes.currentTheme.background
            border.color: Themes.currentTheme.primary
            anchors.bottom: bottomContainer.bottom
            visible: rightBar.currentMode == "layers"

            Rectangle {
                id: addButton
                width: 40
                height: 40
                color: "transparent"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        layerManager.addLayer("Новий шар", rightBar.layerWidth, rightBar.layerHeight, Qt.rgba(0, 0, 0, 0));
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Icons/64x64/add.png"
                        sourceSize: Qt.size(32, 32)
                    }
                    cursorShape: Qt.PointingHandCursor 
                }
            }
            
            Rectangle {
                Layout.fillWidth: true
                width: parent.width - 100
                height: 40
                anchors.centerIn: parent
                color: "transparent"

                Rectangle {
                    id: upArrowButton
                    width: 40
                    height: 40
                    color: "transparent"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (layersList.currentIndex > 0) {
                                layerManager.moveLayer(layersList.currentIndex, layersList.currentIndex - 1);
                                layersList.currentIndex -= 1;
                            }
                        }
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/Icons/64x64/up-arrow.png"
                            sourceSize: Qt.size(32, 32)
                        }
                        cursorShape: Qt.PointingHandCursor 
                    }
                }
                
                Rectangle {
                    id: downArrowButton
                    width: 40
                    height: 40
                    color: "transparent"
                    anchors.left: upArrowButton.right
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (layersList.currentIndex < layersList.count - 1 && layersList.currentIndex >= 0) {
                                layerManager.moveLayer(layersList.currentIndex, layersList.currentIndex + 1);
                                layersList.currentIndex += 1;
                            }
                        }
                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/Icons/64x64/down-arrow.png"
                            sourceSize: Qt.size(32, 32)
                        }
                        cursorShape: Qt.PointingHandCursor 
                    }
                }
            }
            
            Rectangle {
                id: trashButton
                width: 40
                height: 40
                color: "transparent"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        layerManager.removeLayer(layersList.currentIndex);
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Icons/64x64/trash.png"
                        sourceSize: Qt.size(32, 32)
                    }
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        ListView {
            id: layersList
            width: bottomContainer.width
            height: bottomContainer.height - layerContainerBottomMenu.height - layerContainerTopMenu.height
            model: layerManager ? layerManager : undefined
            anchors.top: layerContainerTopMenu.bottom
            clip: true
            visible: rightBar.currentMode == "layers"
            delegate: Rectangle {
                width: parent ? parent.width : rightBar.width
                height: 40
                color: Themes.currentTheme.background
                border.color: ListView.isCurrentItem ? Themes.currentTheme.accent : Qt.darker(Themes.currentTheme.background, 0.5)

                Rectangle {
                    width: 40
                    height: 40
                    color: "transparent"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            layerManager.setLayerVisible(index, !LayerVisible);
                        }
                        Image {
                            anchors.centerIn: parent
                            source: LayerVisible ? "qrc:/Icons/64x64/open-eye.png" : "qrc:/Icons/64x64/closed-eye.png"
                            sourceSize: Qt.size(24, 24)
                        }

                        cursorShape: Qt.PointingHandCursor 
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    width: parent.width - 100
                    height: 40
                    anchors.centerIn: parent
                    color: "transparent"
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layersList.currentIndex = index;
                        }

                        cursorShape: Qt.PointingHandCursor 

                        Text {
                            anchors.centerIn: parent
                            text: name
                            color: "white"
                        }
                    }
                }

                Rectangle {
                    width: 40
                    height: 40
                    color: "transparent"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerManager.setLayerLocked(index, !locked);
                        }
                        Image {
                            anchors.centerIn: parent
                            source: locked ? "qrc:/Icons/64x64/locked.png" : "qrc:/Icons/64x64/unlocked.png"
                            sourceSize: Qt.size(32, 32)
                        }
                        cursorShape: Qt.PointingHandCursor 
                    }
                }
            }
        }

        Rectangle {
            id: brushTopPanel
            width: bottomContainer.width
            height: 50
            color: Themes.currentTheme.background
            border.color: Qt.darker(Themes.currentTheme.background, 0.6)
            anchors.top: bottomContainer.top
            visible: rightBar.currentMode == "brushes"

            Rectangle {
                id: addBrushTile
                width: 50
                height: 50
                color: Themes.currentTheme.background 
                border.color: Qt.darker(Themes.currentTheme.background, 0.6)
                anchors.verticalCenter: brushTopPanel.verticalCenter
                anchors.right: brushTopPanel.right

                Image {
                    source: "qrc:/Icons/64x64/add-brush.png"
                    anchors.centerIn: parent
                    sourceSize: Qt.size(32, 32)
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: brushPopup.open()
                }
            }
        }

        ListView {
            id: brushesList
            width: bottomContainer.width
            height: bottomContainer.height - brushTopPanel.height
            model: brushManager ? brushManager : undefined
            anchors.top: brushTopPanel.bottom
            clip: true
            visible: rightBar.currentMode == "brushes"

            delegate: Rectangle {
                width: parent.width
                height: 64
                Text {
                    text: model.name
                    anchors.centerIn: parent
                }
            }
        }
    }

    Popup {
        id: brushPopup
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        width: Screen.width
        height: Screen.height
        anchors.centerIn: Overlay.overlay

        background: Rectangle {
            color: Themes.currentTheme.background
            border.color: Themes.currentTheme.accent
        }
        
        Column {
            id: brushSettings
            width: brushPopup.width * 0.10
            height: brushPopup.height

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            
            Rectangle {
                width: brushSettings.width
                height: brushSettings.height
                border.color: Qt.darker(Themes.currentTheme.background, 0.6)
                color: Themes.currentTheme.background
            }
        }
        Column {
            id: valuesColumn
            anchors.left: brushSettings.right

            TextField {
                id: brushName
                placeholderText: "Назва пензля"
            }

            Button {
                text: "Оберіть колір"
                onClicked: colorPicker.open()
            }
            
            Row {
                spacing: 10

                Button {
                    text: "Створити"
                    onClicked: brushPopup.close()
                }
                Button {
                    text: "Закрити"
                    onClicked: brushPopup.close()
                }
            }
        }
    }


    Behavior on x {
        NumberAnimation {
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
    
    Column {
        id: buttonColumn
        spacing: 10 
        anchors.verticalCenter: rightBar.verticalCenter
        anchors.right: rightBar.left
        width: 40

        Button {
            id: toggleButton
            width: buttonColumn.width
            height: 80

            property string initialText: "▶"
            text: initialText

            background: Rectangle {
                color: currentTheme.background
                border.color: currentTheme.accent
                radius: 4
            }

            onClicked: {
                rightBar.isVisible = !rightBar.isVisible
                text = rightBar.isVisible ? "▶" : "◀"
                brushButton.visible = rightBar.isVisible
                layerButton.visible = rightBar.isVisible
            }

            Component.onCompleted: {
                text = rightBar.isVisible ? "▶" : "◀"
            }
        }

        Button {
            id: brushButton
            width: buttonColumn.width 
            height: 50 
            visible: false
            
            background: Rectangle {
                color: currentTheme.background
                border.color: currentTheme.accent
                radius: 4
            }

            Image {
                source: "qrc:/Icons/64x64/menu-brush.png"
                sourceSize: Qt.size(32, 32)
                anchors.centerIn: parent
            }

            onClicked: {
                rightBar.currentMode = "brushes";
            }
        }

        Button {
            id: layerButton
            width: buttonColumn.width 
            height: 50
            visible: false
            
            background: Rectangle {
                color: currentTheme.background
                border.color: currentTheme.accent
                radius: 4
            }

            Image {
                source: "qrc:/Icons/64x64/menu-layer.png"
                sourceSize: Qt.size(32, 32)
                anchors.centerIn: parent
            }

            onClicked: {
                rightBar.currentMode = "layers";
            }
        }
    }
}
