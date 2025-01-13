#include "LayerManager.hpp"

namespace harmoniq { namespace layer {

LayerManager::LayerManager(QObject *parent) : QObject(parent) {}

void LayerManager::addLayer(const QString &lname, const QColor &background)
{
    auto newLayer = std::make_shared<Layer>();
    newLayer->setBackgroundColor(background);
    m_layers.append(newLayer);
    m_layerNames.append(lname);
    emit layersChanged();
}

void LayerManager::removeLayer(const int &index)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers.removeAt(index);
        m_layerNames.removeAt(index);
        emit layersChanged();
    }
}

QStringList LayerManager::getLayerNames() const { return m_layerNames; }

std::shared_ptr<Layer> LayerManager::getCurrentLayer(const int &index) const
{
    if (index >= 0 && index < m_layers.size()) {
        return m_layers[index];
    }
    return nullptr;
}
}} // namespace harmoniq::layer
