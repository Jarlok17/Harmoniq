import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import Harmoniq 1.0

Rectangle {
    color: Qt.darker(Themes.currentTheme.background, 0.6)
    anchors.fill: parent

    TabBar {
        id: tabBar
        width: parent.width

        Repeater {
            model: documentManager

            delegate: TabButton {
                id: tabButton
                property bool isModified: documentManager.get(index) ? documentManager.get(index).modified : false
                text: {
                    var baseName = name || "Untitled";
                    return isModified ? baseName + "*" : baseName;
                }
                checked: documentManager.currentIndex === index
                checkable: true
                implicitHeight: 32
                implicitWidth: Math.max(100, text.length * 7 + 30)

                font.pixelSize: 12

                background: Rectangle {
                    radius: 6
                    border.color: tabButton.checked ? Themes.currentTheme.accent : "transparent"
                    color: tabButton.checked ? Qt.darker(Themes.currentTheme.background, 0.7) : Themes.currentTheme.background
                }

                contentItem: Item {
                    anchors.fill: parent

                    Text {
                        id: tabText
                        text: tabButton.text
                        color: "white"
                        font.pixelSize: 12
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        anchors.right: closeButton.left
                        anchors.rightMargin: 8
                        elide: Text.ElideRight
                    }

                    Button {
                        id: closeButton
                        text: "×"
                        font.pixelSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 2
                        background: Rectangle {
                            color: "transparent"
                        }
                        onClicked: documentManager.removeDocument(index)
                    }
                }
                onClicked: documentManager.setCurrentIndex(index)

                Connections {
                    target: documentManager.get(index)
                    function onModifiedChanged() {
                        tabButton.isModified = documentManager.get(index).modified;
                    }
                }
            }
        }
    }
}
