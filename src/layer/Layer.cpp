#include "Layer.hpp"

#include "LayerRenderer.hpp"

namespace harmoniq { namespace layer {

QQuickFramebufferObject::Renderer *Layer::createRenderer() const
{
    auto renderer = new LayerRenderer();
    renderer->setBackgroundColor(m_backgroundColor);
    return renderer;
}

void Layer::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void Layer::setVisibility(const bool &visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        visibleChanged();
        update();
    }
}

QColor Layer::backgroundColor() const { return m_backgroundColor; }

}} // namespace harmoniq::layer
