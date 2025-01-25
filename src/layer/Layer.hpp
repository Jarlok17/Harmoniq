#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickFramebufferObject
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(bool isVisible READ isVisible WRITE setVisibility NOTIFY visibleChanged)
    public:
        Layer() = default;
        Renderer *createRenderer() const override;

        void setBackgroundColor(const QColor &color);
        void setVisibility(const bool &visible);

        QColor backgroundColor() const;
        bool isVisible() const { return m_visible; }
    signals:
        void backgroundColorChanged();
        void visibleChanged();

    private:
        QColor m_backgroundColor = QColor(255, 255, 255, 255);
        bool m_visible = true;
};

}} // namespace harmoniq::layer
