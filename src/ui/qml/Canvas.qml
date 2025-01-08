import QtQuick 2.15
import Harmoniq_backend 1.0

Flickable {
    id: flickableArea
    anchors.fill: parent
    clip: true
    contentWidth: Math.max(canvas.width * canvas.scale, width)
    contentHeight: Math.max(canvas.height * canvas.scale, height)
    interactive: true

    property alias canvas: canvas

    Rectangle {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"

        CanvasGL {
            id: canvas
            width: 800
            height: 600
            scale: 1.0
            opacity: 1.0
            anchors.centerIn: parent

            transform: Scale {
                id: canvasScale
                origin.x: canvas.width / 2
                origin.y: canvas.height / 2
                xScale: canvas.scale
                yScale: canvas.scale
            }

            property real targetScale: 1.0

            WheelHandler {
                id: wheelHandler
                target: canvas
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

                onWheel: function(wheel) {
                    const scaleStep = 0.05;
                    const maxScale = 3.0;
                    const minScale = 0.3;

                    if (wheel.angleDelta.y > 0) {
                        canvas.targetScale = Math.min(canvas.targetScale + scaleStep, maxScale);
                    } else if (wheel.angleDelta.y < 0) {
                        canvas.targetScale = Math.max(canvas.targetScale - scaleStep, minScale);
                    }

                    canvas.scale = canvas.targetScale;
                }
            }
        }
    }
}
