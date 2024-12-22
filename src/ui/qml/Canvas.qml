import QtQuick 2.15
import QtQuick.Controls 2.15

Flickable {
    id: flickableArea
    anchors.fill: parent
    clip: true
    contentWidth: canvas.width > width ? canvas.width : width
    contentHeight: canvas.height > height ? canvas.height : height

    property alias canvas: canvas
    property bool enableFlick: false

    interactive: enableFlick

    Rectangle {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"

        Canvas {
            id: canvas
            anchors.centerIn: parent    
            scale: 1
            clip: true

            property int lastX: 0
            property int lastY: 0
            property bool isFirstDraw: true

            property int canvasWidth: 800
            property int canvasHeight: 600
            property string backgroundColor: "white"

            onCanvasWidthChanged: clear()
            onCanvasHeightChanged: clear()
            onBackgroundColorChanged: clear()

            width: canvasWidth
            height: canvasHeight

            function clear() {
                var ctx = getContext("2d");
                ctx.fillStyle = backgroundColor;
                ctx.fillRect(0, 0, width, height);
                requestPaint();
            }

            MouseArea {
                id: area
                anchors.fill: parent

                onPressed: {
                    if (!flickableArea.enableFlick) {
                        canvas.lastX = mouseX;
                        canvas.lastY = mouseY;
                    }
                }

                onPositionChanged: {
                    if (!flickableArea.enableFlick) {
                        canvas.requestPaint();
                    }
                }
            }

            WheelHandler {
                id: wheelHandler
                target: canvas
                acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

                onWheel: function(wheel) {
                    const scaleFactor = 0.1;
                    if (wheel.angleDelta.y > 0 && canvas.scale < 3.0) {
                        canvas.scale += scaleFactor;
                    } else if (wheel.angleDelta.y < 0 && canvas.scale > 0.3) {
                        canvas.scale -= scaleFactor;
                    }
                }
            }

            onPaint: {
                var ctx = getContext("2d");

                if (isFirstDraw) {
                    ctx.fillStyle = backgroundColor;
                    ctx.fillRect(0, 0, width, height);
                    isFirstDraw = false;
                }
            }
        }
    }
}
