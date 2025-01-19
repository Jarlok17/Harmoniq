#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

    public:
        explicit Layer(QQuickItem *parent = nullptr);

        QColor backgroundColor() const { return m_backgroundColor; }
        void setBackgroundColor(const QColor &color);

    signals:
        void backgroundColorChanged();

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

    private:
        QColor m_backgroundColor;
};

}} // namespace harmoniq::layer
