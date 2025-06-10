#pragma once

#include "ToolHandler.hpp"
#include "ToolSettings.hpp"

namespace harmoniq {

class ColorPickerTool : public ToolHandler
{
    public:
        ColorPickerTool() : canvas(nullptr) {}

        void press(QMouseEvent *event) override
        {
            if (!canvas) {
                qWarning() << "ColorPickerTool: canvas is null!";
                return;
            }

            QPoint pos = event->position().toPoint();

            if (canvas->rect().contains(pos)) {
                QColor pickedColor = canvas->pixelColor(pos);
                ToolSettings::instance()->setColor(pickedColor);
                qDebug() << "ColorPickerTool: Picked color" << pickedColor.name() << "at" << pos;
            } else {
                qWarning() << "ColorPickerTool: Click outside image bounds!";
            }
        }

        void move(QMouseEvent *event) override { Q_UNUSED(event); }
        void release(QMouseEvent *event) override { Q_UNUSED(event); }
        void setToolInfo(const ToolInfo &tool) override { m_tool = tool; }
        void setImage(QImage *image) override { canvas = image; }

        QImage image() const override { return *canvas; }

    private:
        ToolInfo m_tool;
        QImage *canvas;
};

} // namespace harmoniq
