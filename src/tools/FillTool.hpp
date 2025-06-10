#pragma once

#include "../config/precompiled.hpp"
#include "ToolHandler.hpp"

namespace harmoniq {
class FillTool : public ToolHandler
{
    public:
        FillTool() : canvas(nullptr) {}

        void press(QMouseEvent *event) override
        {
            if (!canvas) {
                qWarning() << "FillTool: Canvas is null in press!";
                return;
            }

            QPoint point = event->position().toPoint();
            if (!canvas->rect().contains(point)) {
                qWarning() << "FillTool: Point out of bounds!";
                return;
            }
            if (m_tool.selection.isValid() && !m_tool.selection.contains(point)) {
                qDebug() << "FillTool: Point outside selection, skipping!";
                return;
            }

            QColor targetColor = canvas->pixelColor(point);
            if (targetColor == m_tool.color) {
                return;
            }

            floodFill(point, targetColor);
            qDebug() << "FillTool: Filled at" << point;
        }

        void move(QMouseEvent *event) override { Q_UNUSED(event); }
        void release(QMouseEvent *event) override { Q_UNUSED(event); }
        void setImage(QImage *image) override { canvas = image; }

        QImage image() const override { return *canvas; }
        void setToolInfo(const ToolInfo &info) override
        {
            m_tool.size = info.size;
            m_tool.color = info.color;
            m_tool.opacity = info.opacity;
            m_tool.selection = info.selection;
        }

    private:
        QImage *canvas;
        ToolInfo m_tool;

        void floodFill(const QPoint &start, const QColor &targetColor)
        {
            const QRgb targetRgb = targetColor.rgba();
            const QRgb fillRgb = m_tool.color.rgba();

            if (targetRgb == fillRgb)
                return;

            QStack<QPoint> stack;
            QSet<QPoint> visited;

            stack.push(start);

            while (!stack.isEmpty()) {
                QPoint p = stack.pop();

                if (!canvas->rect().contains(p) || visited.contains(p) ||
                    (m_tool.selection.isValid() && !m_tool.selection.contains(p))) {
                    continue;
                }

                QRgb current = canvas->pixel(p);
                if (!isRgbSimilar(current, targetRgb))
                    continue;

                canvas->setPixel(p, fillRgb);
                visited.insert(p);

                stack.push(QPoint(p.x() + 1, p.y()));
                stack.push(QPoint(p.x() - 1, p.y()));
                stack.push(QPoint(p.x(), p.y() + 1));
                stack.push(QPoint(p.x(), p.y() - 1));
            }
        }

        bool isRgbSimilar(QRgb c1, QRgb c2, int tolerance = 10)
        {
            return std::abs(qRed(c1) - qRed(c2)) <= tolerance && std::abs(qGreen(c1) - qGreen(c2)) <= tolerance &&
                   std::abs(qBlue(c1) - qBlue(c2)) <= tolerance && std::abs(qAlpha(c1) - qAlpha(c2)) <= tolerance;
        }
};
} // namespace harmoniq
