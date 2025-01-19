#include "Layer.hpp"

namespace harmoniq { namespace layer {
Layer::Layer(QQuickItem *parent) : QQuickItem(parent), m_backgroundColor(Qt::white) { setFlag(ItemHasContents, true); }

void Layer::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

QSGNode *Layer::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    QSGSimpleRectNode *rectNode = static_cast<QSGSimpleRectNode *>(node);
    if (!rectNode) {
        rectNode = new QSGSimpleRectNode();
    }

    rectNode->setRect(boundingRect());
    rectNode->setColor(m_backgroundColor);

    return rectNode;
}
}} // namespace harmoniq::layer
