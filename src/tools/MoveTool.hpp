#pragma once

#include "../config/precompiled.hpp"
#include "ToolHandler.hpp"
#include "ToolSettings.hpp"

namespace harmoniq {

class MoveTool : public ToolHandler
{
    public:
        MoveTool() = default;

        void press(QMouseEvent *event) override
        {
            if (!m_image) {
                qWarning() << "MoveTool: Image is null!";
                return;
            }
            m_startPoint = event->position().toPoint();
            m_offset = QPoint(0, 0);

            m_moveRect = m_selectionRect.isValid() ? m_selectionRect : m_image->rect();
            if (m_moveRect.isValid()) {
                m_movedContent = m_image->copy(m_moveRect);
                QPainter painter(m_image);
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.fillRect(m_moveRect, Qt::transparent);
                painter.end();
                qDebug() << "MoveTool: Press - MoveRect:" << m_moveRect << "StartPoint:" << m_startPoint;
            }
        }

        void move(QMouseEvent *event) override
        {
            if (!m_image || m_movedContent.isNull()) {
                qWarning() << "MoveTool: Cannot move - Image:" << m_image << "MovedContent:" << m_movedContent;
                return;
            }
            m_offset = event->position().toPoint() - m_startPoint;
            qDebug() << "MoveTool: Move - Offset:" << m_offset;
        }

        void release(QMouseEvent *event) override
        {
            if (!m_image || m_movedContent.isNull()) {
                qWarning() << "MoveTool: Cannot release - Image:" << m_image << "MovedContent:" << m_movedContent;
                return;
            }

            QPoint finalPos = event->position().toPoint();
            m_offset = finalPos - m_startPoint;

            QRect canvasRect = m_image->rect();

            if (m_selectionRect.isValid()) {
                int dx = m_offset.x();
                int dy = m_offset.y();
                int maxDx = canvasRect.width() - m_moveRect.width();
                int maxDy = canvasRect.height() - m_moveRect.height();
                if (dx > 0)
                    dx = qMin(dx, maxDx);
                if (dx < 0)
                    dx = qMax(dx, -m_moveRect.left());
                if (dy > 0)
                    dy = qMin(dy, maxDy);
                if (dy < 0)
                    dy = qMax(dy, -m_moveRect.top());

                QPoint adjustedTopLeft = m_moveRect.topLeft() + QPoint(dx, dy);
                QRect targetRect(adjustedTopLeft, m_moveRect.size());

                qDebug() << "Translated rect:" << m_moveRect.translated(m_offset);
                qDebug() << "Adjusted TopLeft:" << adjustedTopLeft;
                qDebug() << "TargetRect:" << targetRect;
                qDebug() << "CanvasRect:" << canvasRect;

                QPainter painter(m_image);
                painter.drawImage(targetRect.topLeft(), m_movedContent);
                painter.end();
            } else {
                QPainter painter(m_image);
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.fillRect(canvasRect, Qt::transparent);
                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                painter.drawImage(m_offset, m_movedContent);
                painter.end();

                qDebug() << "Moved entire canvas with Offset:" << m_offset;
                qDebug() << "CanvasRect:" << canvasRect;
            }

            m_movedContent = QImage();
            m_moveRect = QRect();
            m_offset = QPoint(0, 0);
        }

        void setImage(QImage *image) override
        {
            m_image = image;
            qDebug() << "MoveTool: Image set to:" << m_image;
        }

        QImage image() const override { return *m_image; }

        void setToolInfo(const ToolInfo &info) override
        {
            m_info.size = info.size;
            m_info.color = info.color;
            m_info.opacity = info.opacity;
            m_selectionRect = info.selection;
        }

    private:
        QImage *m_image = nullptr;
        ToolInfo m_info;
        QRect m_selectionRect;
        QRect m_moveRect;
        QImage m_movedContent;
        QPoint m_startPoint;
        QPoint m_offset;
};

} // namespace harmoniq
