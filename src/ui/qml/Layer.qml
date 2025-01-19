import QtQuick 2.15
import Harmoniq_backend 1.0
import Harmoniq 1.0

Flickable {
    id: flickableArea
    anchors.fill: parent
    clip: true
    interactive: true

    property real targetScale: 1.0

    property real contentW: 800
    property real contentH: 600

    contentWidth: Math.max(contentW * flickableArea.targetScale, width)
    contentHeight: Math.max(contentH * flickableArea.targetScale, height)

    Rectangle {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"
        
        Repeater {
            model: layerManager ? layerManager : undefined
            Component.onCompleted: {
                //console.log("Layer.qml: layerManager =", layerManager)
            }

            delegate: Layer {
                id: dynamicLayer
                width: LayerWidth
                height: LayerHeight
                backgroundColor: LayerColor
                scale: flickableArea.targetScale
                anchors.centerIn: parent

                Component.onCompleted: {
                    flickableArea.contentW = dynamicLayer.width
                    flickableArea.contentH = dynamicLayer.height
                    console.log("CANVAS WIDTH: ", flickableArea.contentW, " AND HEIGHT: ", flickableArea.contentH)
                }

                onBackgroundColorChanged: console.log("Layer backgroundColor:", backgroundColor)

                WheelHandler {
                    id: wheelHandler
                    target: dynamicLayer
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
                    onWheel: function(wheel) {
                        const scaleStep = 0.05;
                        const maxScale = 2.5;
                        const minScale = 0.5;
                        if (wheel.angleDelta.y > 0) {
                            flickableArea.targetScale = Math.min(flickableArea.targetScale + scaleStep, maxScale);
                        } else if (wheel.angleDelta.y < 0) {
                            flickableArea.targetScale = Math.max(flickableArea.targetScale - scaleStep, minScale);
                        }
                        flickableArea.scale = flickableArea.targetScale;
                    }
                }

                transform: Scale {
                    id: canvasScale
                    origin.x: dynamicLayer.width / 2      
                    origin.y: dynamicLayer.height / 2
                    xScale: flickableArea.targetScale
                    yScale: flickableArea.targetScale
                }
            }
        }
    }
}
