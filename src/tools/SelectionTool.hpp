#pragma once

#include "../config/precompiled.hpp"
#include "../layer/Layer.hpp"
#include "ToolHandler.hpp"
#include "ToolSettings.hpp"

namespace harmoniq {

class SelectionTool : public ToolHandler
{
    public:
        SelectionTool() = default;

        void press(QMouseEvent *event) override
        {
            m_startPoint = event->position().toPoint();
            if (m_layer) {
                m_layer->clearSelection();
            }
        }

        void move(QMouseEvent *event) override
        {
            m_endPoint = event->position().toPoint();
            QRect selectionRect = QRect(m_startPoint, m_endPoint).normalized();
            if (m_layer) {
                m_layer->setSelection(selectionRect);
            }
        }

        void release(QMouseEvent *event) override
        {
            m_endPoint = event->position().toPoint();
            QRect selectionRect = QRect(m_startPoint, m_endPoint).normalized();
            if (selectionRect.isValid() && m_image) {
                selectionRect = selectionRect.intersected(m_image->rect());
                if (m_layer) {
                    m_layer->setSelection(selectionRect);
                }
            } else if (m_layer) {
                m_layer->clearSelection();
            }
        }

        void setImage(QImage *image) override { m_image = image; }

        QImage image() const override { return *m_image; }
        void setLayer(layer::Layer *layer) { m_layer = layer; }

        void setToolInfo(const ToolInfo &info) override
        {
            m_info.size = info.size;
            m_info.color = info.color;
            m_info.opacity = info.opacity;
        }

        QRect getSelection() const { return m_selectionRect; }

    private:
        QImage *m_image = nullptr;
        ToolInfo m_info;
        QPoint m_startPoint;
        QPoint m_endPoint;
        QRect m_selectionRect;
        layer::Layer *m_layer = nullptr;
};

} // namespace harmoniq
