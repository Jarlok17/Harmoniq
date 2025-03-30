#pragma once

#include <QQmlListProperty>

#include "../configs/precompiled.hpp"
#include "Layer.hpp"

namespace harmoniq { namespace layer {
class LayerManager : public QAbstractListModel
{
        Q_OBJECT

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

        explicit LayerManager(QObject *parent = nullptr);
        ~LayerManager();

        Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void mergeLayers(const QString &outputFile);
        Q_INVOKABLE void addLayer(const QString &lname, const int &w, const int &h, const QColor &background,
                                  const bool &locked = false);
        Q_INVOKABLE void removeLayer(const int &index);
        Q_INVOKABLE void moveLayer(const int &from, const int &to);
        Q_INVOKABLE bool isLayerLocked(const int &index) const;
        Q_INVOKABLE void setLayerLocked(const int &index, const bool &locked);
        Q_INVOKABLE void setLayerVisible(const int &index, const bool &visible);

        Q_INVOKABLE void setCurrentTool(ToolType tool);
        Q_INVOKABLE ToolType currentTool() const { return m_currentTool; }

    signals:
        void layersChanged();
        void toolChanged(ToolType tool);

    private:
        struct LayerData
        {
                QString name;
                bool locked = false;
                bool visible = true;
                Layer *layer;
        };
        QList<LayerData> m_layers;
        QSize layerSize;
        ToolType m_currentTool = ToolType::None;
};
}} // namespace harmoniq::layer
