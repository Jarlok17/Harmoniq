import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Harmoniq 1.0
import QtQuick.Window

Rectangle {
    color: Qt.darker(Themes.currentTheme.background, 0.6)
    anchors.fill: parent

    Dialog {
        id: confirmCloseDialog
        title: "Save Changes?"
        width: 400
        height: 200
        modal: true
        property int indexToClose: -1

        // Центрування діалогу
        anchors.centerIn: Overlay.overlay

        contentItem: Item {
            id: contentContainer
            anchors.fill: parent
            anchors.margins: 8 // Відступи для уникнення обрізання
            visible: true
            opacity: 1.0

            Text {
                id: dialogText
                text: {
                    if (documentManager && confirmCloseDialog.indexToClose >= 0) {
                        var doc = documentManager.get(confirmCloseDialog.indexToClose);
                        return "The document '" + (doc && doc.name ? doc.name : "Untitled") + "' has unsaved changes. Do you want to save them before closing?";
                    }
                    return "The document has unsaved changes. Do you want to save them before closing?";
                }
                color: "white"
                wrapMode: Text.WordWrap
                font.pixelSize: 14 // Явно задано розмір шрифту
                padding: 10 // Зменшено padding для кращого балансу
                width: parent.width - 16 // Ширина з урахуванням відступів
                height: Math.max(contentHeight + padding * 2, 50) // Динамічна висота з мінімальним значенням
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter 
                visible: true
                opacity: 1.0
                onTextChanged: function(newText) {
                    console.log("Dialog text updated:", newText);
                    console.log("Text size:", width, implicitHeight, "Position:", x, y, "Visible:", visible, "Opacity:", opacity);
                    console.log("Content container size:", contentContainer.width, contentContainer.height);
                }
            }
        }

        background: Rectangle {
            color: Themes.currentTheme.background
            border.color: Themes.currentTheme.accent
            opacity: 1.0
        }

        footer: DialogButtonBox {
            Button {
                text: "Save"
                onClicked: confirmCloseDialog.accept()
            }
            Button {
                text: "Discard"
                onClicked: confirmCloseDialog.reject()
            }
            Button {
                text: "Cancel"
                onClicked: confirmCloseDialog.close()
            }
        }

        onAboutToShow: {
            console.log("Opening dialog for index:", indexToClose, "Document:", documentManager.get(indexToClose));
            console.log("Dialog size:", width, height, "Background color:", Themes.currentTheme.background, "Text color:", dialogText.color);
            console.log("Text size:", dialogText.width, dialogText.implicitHeight, "Position:", dialogText.x, dialogText.y);
            console.log("Content container size:", contentContainer.width, contentContainer.height);
        }

        onVisibleChanged: console.log("Dialog visible changed:", visible)

        onAccepted: {
            console.log("Saving document before closing, index:", indexToClose);
            var doc = documentManager.get(indexToClose);
            const currentPath = doc ? doc.path : "";
            console.log("Save triggered with path:", currentPath);
            if (!currentPath || currentPath.trim() === "") {
                saveDialog.index = indexToClose;
                saveDialog.open();
            } else {
                const success = documentManager.saveToFile(currentPath);
                if (success) {
                    console.log("Saved successfully to:", currentPath);
                    if (doc) {
                        doc.setModified(false);
                        documentManager.removeDocument(indexToClose);
                    }
                } else {
                    console.warn("Save failed, closing cancelled for index:", indexToClose);
                }
            }
        }

        onRejected: {
            console.log("Discarding changes, closing document, index:", indexToClose);
            documentManager.removeDocument(indexToClose);
        }

        onClosed: {
            if (!accepted && !rejected) { // Аналог Dialog.Cancel
                console.log("Closing cancelled for document, index:", indexToClose);
            }
        }
    }
    FileDialog {
        id: saveDialog
        title: "Save Document As"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Harmoniq Documents (*.json)", "All Files (*)"]
        property int index: -1

        onAccepted: {
            if (fileUrl && fileUrl.toString().trim() !== "") {
                var fullPath = fileUrl.toString();
                var localPath = fullPath.replace(/^file:\/\//, "");
                var fileName = localPath.split('/').pop().replace(/\.json$/, "");
                console.log("Saving to:", fullPath, "FileName:", fileName);
                var doc = documentManager.get(index);
                if (doc) {
                    doc.setName(fileName);
                    doc.setPath(fullPath);
                    if (documentManager.saveToFile(fullPath)) {
                        console.log("Document saved successfully, index:", index);
                        doc.setModified(false);
                        documentManager.removeDocument(index);
                    } else {
                        console.warn("Failed to save document, index:", index);
                    }
                } else {
                    console.warn("No document found for index:", index);
                }
            }
        }

        onRejected: {
            console.log("Save dialog cancelled for index:", index);
        }
    }

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
                        onClicked: {
                            if (isModified) {
                                confirmCloseDialog.indexToClose = index;
                                confirmCloseDialog.open();
                            } else {
                                console.log("Closing unmodified document, index:", index);
                                documentManager.removeDocument(index);
                            }
                        }
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
