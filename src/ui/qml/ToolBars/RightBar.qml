import QtQuick 2.15
import QtQuick.Controls 2.15

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

            Row {
                spacing: 10
                anchors.centerIn: parent

                Button {
                    id: button
                    text: "+"
                    background: Rectangle {
                        color: button.down ? "lightblue" : "blue"
                        radius: 10
                        border.color: "black"
                    }
                    onClicked: {
                        layerManager.addLayer("Новий шар", rightBar.layerWidth, rightBar.layerHeight, Qt.rgba(0, 0, 0, 0))
                    }
                }

                Button {
                    text: "---"
                    onClicked: {
                        if (layersList.currentIndex >= 0) {
                            layerManager.removeLayer(layersList.currentIndex)
                        }
                    }
                }
            }
        }

        ListView {
            id: layersList
            width: layerContainer.width
            height: layerContainer.height - layerContainerMenu.height
            model: layerManager ? layerManager : undefined
            Component.onCompleted: {
                //console.log("RightBar.qml: layerManager = ", layerManager); 
            }
            anchors.top: layerContainerMenu.bottom
            delegate: Rectangle {
                width: parent.width
                height: 40
                color: ListView.isCurrentItem ? "lightblue" : "white"
                border.color: "black"
                
                Row {
                    anchors.fill: parent
                    spacing: 10
                    /* 
                    Image {
                        source: Qt.resolvedUrl(thumbnail)
                        width: 40
                        height: 40
                    }*/
                    Text {
                        text: name
                    }

                    Text {
                        text: locked ? "Locked" : "Unlocked"
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
