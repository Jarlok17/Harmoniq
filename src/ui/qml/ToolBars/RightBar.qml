import QtQuick 2.15
import QtQuick.Controls 2.15
import Harmoniq_backend 1.0

Rectangle {
    id: rightBar
    width: 250
    height: parent.height
    color: "#f0f0f0"

    LayersModel { // Модель шарів
        id: layerModel
    }

    Column {
        anchors.fill: parent
        spacing: 10

        // Верхня частина (може використовуватися для інших функцій)
        Rectangle {
            id: upperSection
            height: parent.height / 2
            width: parent.width
            color: "#e0e0e0"
            Text {
                anchors.centerIn: parent
                text: "Інші елементи управління"
            }
        }

        // Нижня частина для відображення шарів
        Rectangle {
            id: lowerSection
            height: parent.height / 2
            width: parent.width
            color: "#d0d0d0"

            Column {
                anchors.fill: parent
                spacing: 10

                Text {
                    text: "Шари"
                    font.pixelSize: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ListView {
                    id: layersList
                    model: layerModel
                    delegate: Item {
                        width: parent.width
                        height: 40

                        Row {
                            spacing: 10

                            Text {
                                text: layerName
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Button {
                                text: "⬆"
                                onClicked: layerModel.moveLayerUp(index)
                            }

                            Button {
                                text: "⬇"
                                onClicked: layerModel.moveLayerDown(index)
                            }
                        }
                    }
                }

                Row {
                    spacing: 10
                    Button {
                        text: "Додати шар"
                        onClicked: root.addLayer()
                    }
                    Button {
                        text: "Видалити шар"
                        onClicked: root.removeLayer(layersList.currentIndex)
                    }
                }
            }
        }
    }
}
