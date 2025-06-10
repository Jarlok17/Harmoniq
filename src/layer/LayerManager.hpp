#pragma once

#include "../config/precompiled.hpp"
#include "Layer.hpp"

namespace harmoniq { namespace layer {
class LayerManager : public QAbstractListModel
{
        Q_OBJECT
        Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    public:
        enum LayerRoles {
            NameRole = Qt::UserRole + 1,
            LockedRole,
            VisibleRole,
            WidthRole,
            HeightRole,
            ColorRole,
            LayerObjectRole,
        };

        struct LayerData
        {
                QString name;
                bool locked = false;
                bool visible = true;
                Layer *layer;
        };

        explicit LayerManager(QObject *parent = nullptr);
        ~LayerManager();

        Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void addLayer(const QString &lname, const int &w, const int &h, const QColor &background,
                                  const bool &locked = false);
        Q_INVOKABLE void removeLayer(const int &index);
        Q_INVOKABLE void moveLayer(const int &from, const int &to);
        Q_INVOKABLE bool isLayerLocked(const int &index) const;
        Q_INVOKABLE void setLayerLocked(const int &index, const bool &locked);
        Q_INVOKABLE void setLayerVisible(const int &index, const bool &visible);
        Q_INVOKABLE void setCurrentTool(ToolType tool);
        Q_INVOKABLE void setCurrentIndex(int index);
        Q_INVOKABLE void setLayerName(const int &index, const QString &name);
        Q_INVOKABLE QImage getMergedImage() const;
        Q_INVOKABLE Layer *getCurrentLayer() const;
        LayerManager::LayerData *get(int index);

        Q_INVOKABLE ToolType currentTool() const { return m_currentTool; }
        Q_INVOKABLE int currentIndex() const { return m_currentIndex; }

    signals:
        void layersChanged();
        void toolChanged(ToolType tool);
        void currentIndexChanged();
        void layerAdded(Layer *layer);

    private:
        int m_currentIndex = -1;
        QList<LayerData> m_layers;
        QSize layerSize;
        ToolType m_currentTool = ToolType::None;
};
}} // namespace harmoniq::layer
