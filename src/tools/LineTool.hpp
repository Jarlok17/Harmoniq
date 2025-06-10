#pragma once

#include "../config/precompiled.hpp"
#include "ToolHandler.hpp"
#include "ToolSettings.hpp"

namespace harmoniq {

class LineTool : public ToolHandler
{
    public:
        LineTool() = default;

        void press(QMouseEvent *event) override
        {
            if (!m_image)
                return;
            m_startPoint = event->position().toPoint();
            m_currentInfo = m_info;
        }

        void move(QMouseEvent *event) override
        {
            if (!m_image)
                return;
            m_endPoint = event->position().toPoint();
        }

        void release(QMouseEvent *event) override
        {
            if (!m_image)
                return;
            m_endPoint = event->position().toPoint();

            QPoint pos = event->position().toPoint();
            if (m_info.selection.isValid() && !m_info.selection.contains(pos)) {
                qDebug() << "BrushTool: Position outside selection, skipping!";
                return;
            }

            QPainter painter(m_image);
            painter.setRenderHint(QPainter::Antialiasing, true);

            if (m_info.selection.isValid()) {
                painter.setClipRect(m_info.selection);
            }

            QPen pen(m_currentInfo.color);
            pen.setWidthF(m_currentInfo.size);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);
            painter.setOpacity(m_currentInfo.opacity);

            painter.drawLine(m_startPoint, m_endPoint);
            painter.end();
        }

        void setImage(QImage *image) override { m_image = image; }

        QImage image() const override { return *m_image; }

        void setToolInfo(const ToolInfo &info) override
        {
            m_info.size = info.size;
            m_info.color = info.color;
            m_info.opacity = info.opacity;
            m_info.selection = info.selection;
        }

    private:
        QImage *m_image = nullptr;
        ToolInfo m_info;
        ToolInfo m_currentInfo;
        QPoint m_startPoint;
        QPoint m_endPoint;
};

} // namespace harmoniq
