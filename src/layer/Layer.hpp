#pragma once

#include <functional>

#include "../tools/ToolHandler.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickPaintedItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(bool isVisible READ isVisible WRITE setVisibility NOTIFY visibleChanged)
    public:
        explicit Layer(QQuickItem *parent = nullptr);
        ~Layer();

        void updateCanvasSize();

        Q_INVOKABLE void setBackgroundColor(const QColor &color);
        Q_INVOKABLE void setVisibility(const bool &visible);
        Q_INVOKABLE void setActive(const bool &active);
        void setToolInfo(const ToolInfo &info);
        Q_INVOKABLE void setCurrentTool(ToolType type);
        Q_INVOKABLE void setLocked(const bool &locked);
        Q_INVOKABLE void setImage(const QImage &image);
        void setSelection(const QRect &rect);
        void clearSelection();

        QColor backgroundColor() const { return m_backgroundColor; }
        bool isVisible() const { return m_visible; }
        bool isLocked() const { return m_locked; }
        bool isActive() const { return m_active; }
        ToolInfo tool() const { return m_tool; }
        QImage image() const { return m_canvas; }
        QRect getSelection() const { return m_currentSelection; }

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void paint(QPainter *painter) override;
        void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    signals:
        void backgroundColorChanged();
        void visibleUpdated(bool visible);
        void lockedChanged();
        void activeChanged();
        void stateChanged(const QImage &image, Layer *layer);
        void imageChanged(const QImage &image);

    private:
        QImage m_canvas;
        QColor m_backgroundColor;
        ToolInfo m_tool;
        bool m_visible = true;
        bool m_active = false;
        bool m_locked = false;
        bool m_initialized = false;

        ToolType m_currentTool = ToolType::None;
        std::unique_ptr<ToolHandler> m_toolHandler;
        QRect m_currentSelection;
};

}} // namespace harmoniq::layer
