import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Dialogs

import Harmoniq 1.0
import Harmoniq_backend 1.0

Rectangle {
    id: rightBar
    width: Screen.width * 0.16
    height: parent.height
    color: Themes.currentTheme.background
    border.color: Qt.darker(Themes.currentTheme.background, 0.5)

    property bool isVisible: false
    property real hiddenX: parent.width
    property real visibleX: parent.width - width

    property double layerWidth: 800 
    property double layerHeight: 600

    Rectangle {
        id: colorContainer
        width: rightBar.width
        height: 0
        color: Themes.currentTheme.background
        border.color: Themes.currentTheme.accent
        anchors.top: rightBar.top

        Loader {
            id: colorPickerLoader
            anchors.fill: parent
            source: "qrc:/UI/ColorPicker.qml"

            onLoaded: {
                if (colorPickerLoader.item) {
                    ToolSettings.color = colorPickerLoader.item.selectedColor
                }
            }
        }

        Connections {
            target: colorPickerLoader.item
            onHueChanged: ToolSettings.color = colorPickerLoader.item.selectedColor
            onSaturationChanged: ToolSettings.color = colorPickerLoader.item.selectedColor
            onValueChanged: ToolSettings.color = colorPickerLoader.item.selectedColor
            onAlphaChanged: ToolSettings.color = colorPickerLoader.item.selectedColor
        } 
    }

    Rectangle {
        id: bottomContainer
        width: rightBar.width
        height: rightBar.height / 2
        color: Themes.currentTheme.background
        border.color: Qt.darker(Themes.currentTheme.background, 0.5)
        anchors.bottom: rightBar.bottom

        Rectangle {
            id: layerContainerBottomMenu
            width: bottomContainer.width
            height: 50
            color: Themes.currentTheme.background
            border.color: Qt.darker(Themes.currentTheme.background, 0.5)
            anchors.bottom: bottomContainer.bottom

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
                        var layerName = "New Layer";
                        documentManager.currentLayerManager.addLayer(layerName + " " + documentManager.currentLayerManager.rowCount(), rightBar.layerWidth, rightBar.layerHeight, Qt.rgba(0, 0, 0, 0));
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
                                documentManager.currentLayerManager.moveLayer(layersList.currentIndex, layersList.currentIndex - 1);
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
                                documentManager.currentLayerManager.moveLayer(layersList.currentIndex, layersList.currentIndex + 1);
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
                        documentManager.currentLayerManager.removeLayer(layersList.currentIndex);
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
            height: bottomContainer.height - layerContainerBottomMenu.height
            model: documentManager.current ? documentManager.currentLayerManager : null
            anchors.top: bottomContainer.top
            clip: true

            onCountChanged: {
                console.log("Layers count:", layersList.count);
            }

            Connections {
                target: documentManager.currentLayerManager
                ignoreUnknownSignals: true
                function onCurrentIndexChanged() {
                    const index = documentManager.currentLayerManager.currentIndex;
                    console.log("QML: LayerManager currentIndexChanged to:", index);
                    layersList.currentIndex = index;
                }
            }

            Connections {
                target: documentManager
                function onLayerManagerChanged() {
                    console.log("QML: LayerManager set");
                    if (documentManager.currentLayerManager) {
                        layersList.currentIndex = documentManager.currentLayerManager.currentIndex;
                        console.log("QML: Initial currentIndex set to:", layersList.currentIndex);
                    }
                }
            }

            delegate: Rectangle {
                width: parent ? parent.width : rightBar.width
                height: 40
                color: Themes.currentTheme.background
                border.color: ListView.isCurrentItem ? "white" : Qt.darker(Themes.currentTheme.background, 0.5)

                property bool isEditing: false

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
                            console.log("Toggling visibility for layer:", index);
                            documentManager.currentLayerManager.setLayerVisible(index, !model.visible);
                        }
                        Image {
                            anchors.centerIn: parent
                            source: model.visible ? "qrc:/Icons/64x64/open-eye.png" : "qrc:/Icons/64x64/closed-eye.png"
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

                    TextField {
                        id: layerNameField
                        visible: parent.parent.isEditing
                        anchors.fill: parent
                        anchors.margins: 5
                        text: name
                        color: "white"
                        background: Rectangle {
                            color: Themes.currentTheme.background
                            border.color: Themes.currentTheme.accent
                            radius: 4
                        }
                        onEditingFinished: {
                            if (text.trim() !== "") {
                                documentManager.currentLayerManager.setLayerName(index, text.trim());
                            }
                            console.log("Editing finished, setting isEditing to false");
                            parent.parent.isEditing = false;
                            Qt.callLater(function() {
                                layerNameField.visible = false;
                            });
                        }
                        onFocusChanged: {
                            if (!focus) {
                                console.log("Focus lost, setting isEditing to false");
                                parent.parent.isEditing = false;
                                Qt.callLater(function() {
                                    layerNameField.visible = false;
                                });
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            console.log("Double-clicked on layer:", index);
                            parent.parent.isEditing = true;
                            layerNameField.visible = true;
                            layerNameField.forceActiveFocus();
                            layerNameField.selectAll();
                        }
                        onClicked: {
                            console.log("Selecting layer with index:", index);
                            layersList.currentIndex = index;
                            documentManager.currentLayerManager.setCurrentIndex(index);
                        }
                        cursorShape: Qt.PointingHandCursor 

                        Text {
                            visible: !parent.parent.isEditing
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
                            console.log("Toggling lock for layer:", index);
                            documentManager.currentLayerManager.setLayerLocked(index, !locked);
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
    }

    Behavior on x {
        NumberAnimation {
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
}
