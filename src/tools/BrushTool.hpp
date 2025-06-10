#pragma once

#include "ToolHandler.hpp"

namespace harmoniq {
class BrushTool : public ToolHandler
{
    public:
        BrushTool() : canvas(nullptr), isDrawing(false) {}

        void press(QMouseEvent *event) override
        {
            if (!canvas) {
                qWarning() << "BrushTool: Canvas is null in press!";
                return;
            }
            isDrawing = true;
            m_tool.position = QVector2D(event->position());
            qDebug() << "BrushTool: Press at" << event->position();
        }

        void move(QMouseEvent *event) override
        {
            if (!isDrawing || !canvas) {
                qWarning() << "BrushTool: Cannot draw - isDrawing:" << isDrawing << "canvas:" << canvas;
                return;
            }

            QPoint pos = event->position().toPoint();
            if (m_tool.selection.isValid() && !m_tool.selection.contains(pos)) {
                qDebug() << "BrushTool: Position outside selection, skipping!";
                return;
            }

            QPainter painter(canvas);
            if (!painter.isActive()) {
                qWarning() << "BrushTool: Painter is not active!";
                return;
            }

            if (m_tool.selection.isValid()) {
                painter.setClipRect(m_tool.selection);
            }

            painter.setOpacity(m_tool.opacity);
            painter.setPen(QPen(m_tool.color, m_tool.size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(QPointF(m_tool.position.x(), m_tool.position.y()), event->position());
            m_tool.position = QVector2D(event->position());
            qDebug() << "BrushTool: Move at" << event->position() << "on canvas:" << canvas;
        }

        void release(QMouseEvent *event) override
        {
            Q_UNUSED(event);
            isDrawing = false;
            qDebug() << "BrushTool: Release";
        }

        void setToolInfo(const ToolInfo &tool) override
        {
            m_tool.size = tool.size;
            m_tool.color = tool.color;
            m_tool.opacity = tool.opacity;
            m_tool.hardness = tool.hardness;
            m_tool.selection = tool.selection;
            qDebug() << "BrushTool: Brush set with color:" << m_tool.color;
        }

        void setImage(QImage *image) override
        {
            canvas = image;
            qDebug() << "BrushTool: Image set to:" << canvas;
            if (canvas) {
                qDebug() << "BrushTool: Canvas size:" << canvas->size();
            } else {
                qWarning() << "BrushTool: Set null canvas!";
            }
        }

        QImage image() const override { return *canvas; }

    private:
        ToolInfo m_tool;
        QImage *canvas;
        bool isDrawing;
};

} // namespace harmoniq
