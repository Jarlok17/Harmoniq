#pragma once

#include "../config/precompiled.hpp"
#include "ToolHandler.hpp"

namespace harmoniq {

enum class ShapeType {
    Rectangle,
    Ellipse
};

class ShapeTool : public ToolHandler
{
    public:
        ShapeTool() : m_image(nullptr), m_shapeType(ShapeType::Rectangle) {}

        void setShapeType(ShapeType type) { m_shapeType = type; }

        void setImage(QImage *image) override { m_image = image; }

        QImage image() const override { return *m_image; }
        void setToolInfo(const ToolInfo &info) override { m_tool = info; }

        void press(QMouseEvent *event) override
        {
            m_start = event->pos();
            m_end = m_start;
            m_drawing = true;
        }

        void move(QMouseEvent *event) override
        {
            if (m_drawing) {
                m_end = event->pos();
            }
        }

        void release(QMouseEvent *event) override
        {
            if (!m_image || !m_drawing)
                return;

            m_end = event->pos();
            m_drawing = false;

            QPainter painter(m_image);
            painter.setRenderHint(QPainter::Antialiasing, true);
            QColor color = m_tool.color;
            color.setAlphaF(m_tool.opacity);

            QPen pen(color, m_tool.size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            QBrush brush = m_tool.fill ? QBrush(m_tool.color) : Qt::NoBrush;
            painter.setBrush(brush);

            QRectF rect(m_start, m_end);
            rect = rect.normalized();

            switch (m_shapeType) {
            case ShapeType::Rectangle:
                painter.drawRect(rect);
                break;
            case ShapeType::Ellipse:
                painter.drawEllipse(rect);
                break;
            }
        }

    private:
        QImage *m_image;
        ToolInfo m_tool;
        ShapeType m_shapeType;

        QPoint m_start;
        QPoint m_end;
        bool m_drawing = false;
};

} // namespace harmoniq
