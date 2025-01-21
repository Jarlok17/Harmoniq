#pragma once

#include "../brushes/BrushInfo.hpp"
#include "../configs/precompiled.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickPaintedItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(QImage thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)

    public:
        explicit Layer(QQuickPaintedItem *parent = nullptr);

        void setBackgroundColor(const QColor &color);
        void setBrushInfo(const brushes::BrushInfo &info) { m_brushInfo = info; }
        void setThumbnail(const QImage &thumbnail);

        QColor backgroundColor() const { return m_backgroundColor; }
        QImage thumbnail() const { return m_thumbnail; }
        const brushes::BrushInfo &brushInfo() const { return m_brushInfo; }

    signals:
        void backgroundColorChanged();
        void thumbnailChanged();

    protected:
        void paint(QPainter *painter) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

    private:
        QImage m_canvas;
        QColor m_backgroundColor;

        QImage m_thumbnail;

        brushes::BrushInfo m_brushInfo;
};

}} // namespace harmoniq::layer
