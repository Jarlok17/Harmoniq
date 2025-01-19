#pragma once

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
            WidthRole,
            HeightRole,
            ColorRole
        };

        explicit LayerManager(QObject *parent = nullptr);
        ~LayerManager();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void addLayer(const QString &lname, const int &w, const int &h, const QColor &background,
                                  const bool &locked = false);
        Q_INVOKABLE void removeLayer(const int &index);
        Q_INVOKABLE bool isLayerLocked(const int &index) const;
        Q_INVOKABLE void setLayerLocked(const int &index, const bool &locked);

    private:
        struct LayerData
        {
                QString name;
                bool locked = false;
                std::shared_ptr<Layer> layer;
        };
        QVector<LayerData> m_layers;
};
}} // namespace harmoniq::layer
