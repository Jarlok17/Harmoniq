import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Item {
    id: drawingCanvas
    width: 800
    height: 600

    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true

        Canvas {
            id: canvas
            width: 600
            height: 800
            anchors.centerIn: parent

            property int lastX: 0
            property int lastY: 0
            property bool isFirstDraw: true

            function clear() {
                var ctx = getContext("2d");
                ctx.fillStyle = "white";
                ctx.fillRect(0, 0, canvas.width, canvas.height);
                canvas.requestPaint();
            }

            MouseArea {
                id: area
                anchors.fill: parent

                onPressed: {
                    canvas.lastX = mouseX;
                    canvas.lastY = mouseY;
                }

                onPositionChanged: {
                    canvas.requestPaint();
                }
            }

            onPaint: {
                var ctx = getContext("2d");

                if (isFirstDraw) {
                    ctx.fillStyle = "white";
                    ctx.fillRect(0, 0, canvas.width, canvas.height);
                    isFirstDraw = false;
                }

                ctx.lineWidth = 5;
                ctx.strokeStyle = "black";
                ctx.beginPath();
                ctx.moveTo(lastX, lastY);
                lastX = area.mouseX;
                lastY = area.mouseY;
                ctx.lineTo(lastX, lastY);
                ctx.stroke();
            }
        }
    }
}
