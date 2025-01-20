import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15

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

    x: isVisible ? visibleX : hiddenX

    Rectangle {
        id: layerContainer
        width: rightBar.width
        height: rightBar.height / 3
        color: Themes.currentTheme.background
        border.color: Themes.currentTheme.primary
        anchors.bottom: rightBar.bottom

        Rectangle {
            id: layerContainerMenu
            width: layerContainer.width
            height: 50
            color: Themes.currentTheme.background
            border.color: Themes.currentTheme.primary
            anchors.top: layerContainer.top

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
                        console.log("New layer is added");
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

                Button {
                    text: "▲"
                    onClicked: {
                        if (layersList.currentIndex > 0) {
                            layerManager.moveLayer(layersList.currentIndex, layersList.currentIndex - 1);
                            layersList.currentIndex -= 1;
                        }
                    }
                }

                Button {
                    text: "▼"
                    onClicked: {
                        if (layersList.currentIndex < layersList.count - 1 && layersList.currentIndex >= 0) {
                            layerManager.moveLayer(layersList.currentIndex, layersList.currentIndex + 1);
                            layersList.currentIndex += 1;
                        }
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
                        console.log("Layer deleted");
                    }
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Icons/64x64/trash.png"
                        sourceSize: Qt.size(32, 32)
                    }
                }
            }
        }

        ListView {
            id: layersList
            width: layerContainer.width
            height: layerContainer.height - layerContainerMenu.height
            model: layerManager ? layerManager : undefined
            anchors.top: layerContainerMenu.bottom
            clip: true
            delegate: Rectangle {
                width: parent.width
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
                            console.log("Layer visibility:", !LayerVisible);
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
                            console.log("Selected layer:", name);
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
                            console.log("Layer locked:", !locked);
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

    Button {
        id: toggleButton
        width: 40
        height: 100
        anchors.verticalCenter: rightBar.verticalCenter
        anchors.right: rightBar.left

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
        }

        Component.onCompleted: {
            text = rightBar.isVisible ? "▶" : "◀"
        }
    }
}
