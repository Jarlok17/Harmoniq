#pragma once

#include "../history/CanvasHistoryManager.hpp"
#include "ToolHandler.hpp"

namespace harmoniq {
class EraseTool : public ToolHandler
{
    public:
        EraseTool() : canvas(nullptr), isErasing(false) {}

        void press(QMouseEvent *event) override
        {
            if (!canvas) {
                qWarning() << "EraseTool: Canvas is null in press!";
                return;
            }
            isErasing = true;
            m_tool.position = QVector2D(event->position());
            qDebug() << "EraseTool: Press at" << event->position();
        }

        void move(QMouseEvent *event) override
        {
            if (!isErasing || !canvas) {
                qWarning() << "EraseTool: Cannot erase - isErasing:" << isErasing << "canvas:" << canvas;
                return;
            }

            QPoint pos = event->position().toPoint();
            if (m_tool.selection.isValid() && !m_tool.selection.contains(pos)) {
                qDebug() << "EraseTool: Position outside selection, skipping!";
                return;
            }

            QPainter painter(canvas);
            if (!painter.isActive()) {
                qWarning() << "EraseTool: Painter is not active!";
                return;
            }

            if (m_tool.selection.isValid()) {
                painter.setClipRect(m_tool.selection);
            }

            painter.setOpacity(m_tool.opacity);
            painter.setPen(QPen(Qt::transparent, m_tool.size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.drawLine(QPointF(m_tool.position.x(), m_tool.position.y()), event->position());
            m_tool.position = QVector2D(event->position());
            qDebug() << "EraseTool: Move at" << event->position() << "on canvas:" << canvas;
        }

        void release(QMouseEvent *event) override
        {
            Q_UNUSED(event);
            isErasing = false;
            qDebug() << "EraseTool: Release";
        }

        void setToolInfo(const ToolInfo &tool) override
        {
            m_tool.size = tool.size;
            m_tool.color = tool.color;
            m_tool.opacity = tool.opacity;
            m_tool.hardness = tool.hardness;
            m_tool.fill = tool.fill;
            m_tool.selection = tool.selection;
            qDebug() << "EraseTool: Brush set with color:" << m_tool.color;
        }

        void setImage(QImage *image) override
        {
            canvas = image;
            qDebug() << "EraseTool: Image set to:" << canvas;
            if (canvas) {
                qDebug() << "EraseTool: Canvas size:" << canvas->size();
            } else {
                qWarning() << "EraseTool: Set null canvas!";
            }
        }

        QImage image() const override { return *canvas; }

    private:
        ToolInfo m_tool;
        QImage *canvas;
        bool isErasing;
};
} // namespace harmoniq
