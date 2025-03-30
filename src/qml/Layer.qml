import QtQuick 2.15
import Harmoniq_backend 1.0
import Harmoniq 1.0

Flickable {
    id: flickableArea
    anchors.fill: parent
    clip: true
    interactive: false

    property real targetScale: 1.0
    property real contentW: 800
    property real contentH: 600

    property real mousePositionX: 0 
    property real mousePositionY: 0

    contentWidth: Math.max(contentW * targetScale, width)
    contentHeight: Math.max(contentH * targetScale, height)

    Rectangle {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"

        Repeater {
            model: layerManager

            delegate: Item {
                id: layerItem
                width: model.width
                height: model.height
                anchors.centerIn: parent
                scale: flickableArea.targetScale

                children: [model.layerObject]

                Component.onCompleted: {
                    flickableArea.contentW = width
                    flickableArea.contentH = height
                    console.log("CANVAS WIDTH:", flickableArea.contentW, "HEIGHT:", flickableArea.contentH)
                }

                WheelHandler {
                    id: wheelHandler
                    target: layerItem 
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
                    origin.x: layerItem.width / 2      
                    origin.y: layerItem.height / 2
                    xScale: flickableArea.targetScale
                    yScale: flickableArea.targetScale
                }
            }
        }
    }
}
