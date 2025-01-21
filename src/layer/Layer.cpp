#include "Layer.hpp"

namespace harmoniq { namespace layer {
Layer::Layer(QQuickPaintedItem *parent) : QQuickPaintedItem(parent), m_backgroundColor(Qt::white)
{
    setFlag(ItemHasContents, true);
    m_canvas = QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
    m_canvas.fill(Qt::transparent);
}

void Layer::mousePressEvent(QMouseEvent *event) {}
void Layer::mouseMoveEvent(QMouseEvent *event) {}

void Layer::paint(QPainter *painter) { painter->fillRect(boundingRect(), m_backgroundColor); }

void Layer::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void Layer::setThumbnail(const QImage &thumbnail)
{
    if (m_thumbnail != thumbnail) {
        m_thumbnail = thumbnail;
        emit thumbnailChanged();
        update();
    }
}
}} // namespace harmoniq::layer
