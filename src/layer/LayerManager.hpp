#pragma once

#include "Layer.hpp"

namespace harmoniq { namespace layer {
class LayerManager : public QObject
{
        Q_OBJECT

    public:
        explicit LayerManager(QObject *parent = nullptr);
        LayerManager(const LayerManager &) = delete;
        LayerManager(LayerManager &&) = delete;

        Q_INVOKABLE void addLayer(const QString &lname, const QColor &background = QColor(255, 255, 255));
        Q_INVOKABLE void removeLayer(const int &index);
        Q_INVOKABLE bool isLayerLocked(const int &index) const;
        Q_INVOKABLE void setLayerLocked(const int &index, const bool &locked);

        Q_INVOKABLE QImage getThumbnail(const int &index) const;
        Q_INVOKABLE std::shared_ptr<Layer> getCurrentLayer(const int &index) const;
        Q_INVOKABLE QStringList getLayerNames() const;

    signals:
        void layersChanged();

    private:
        QVector<std::shared_ptr<Layer>> m_layers;
        QStringList m_layerNames;
};
}} // namespace harmoniq::layer
