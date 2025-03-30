#pragma once

#include "../brushes/BrushInfo.hpp"
#include "../configs/precompiled.hpp"
#include "Tools.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickPaintedItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(bool isVisible READ isVisible WRITE setVisibility NOTIFY visibleChanged)
    public:
        explicit Layer(QQuickItem *parent = nullptr);
        ~Layer();

        void setBackgroundColor(const QColor &color);
        void setVisibility(const bool &visible);
        void setCurrentBrush(const brushes::Brush &brush);
        void setCurrentTool(ToolType type);

        QColor backgroundColor() const;
        bool isVisible() const { return m_visible; }
        brushes::Brush brush() const { return m_brush; }
        QImage image() const { return m_canvas; }

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paint(QPainter *painter) override;
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    signals:
        void backgroundColorChanged();
        void visibleUpdated(bool visible);

    private:
        void updateCanvasSize();

        QImage m_canvas;
        QColor m_backgroundColor;
        brushes::Brush m_brush;
        bool m_visible = true;
        ToolType m_currentTool = ToolType::None;
        bool m_initialized = false;

        std::unique_ptr<ToolHandler> m_toolHandler;
};

}} // namespace harmoniq::layer
