#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <qnamespace.h>

namespace harmoniq { namespace layer {
struct Layer
{
        QString name;
        bool isVisible;
};

class LayersModel : public QAbstractListModel
{
        Q_OBJECT
    public:
        enum LayerRoles {
            NameRole = Qt::UserRole + 1,
            VisibilityRole
        };

        LayersModel(QObject *parent = nullptr);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void addLayer(const QString &name);
        Q_INVOKABLE void removeLayer(int index);
        Q_INVOKABLE void moveLayerUp(int index);
        Q_INVOKABLE void moveLayerDown(int index);

    private:
        QList<Layer> layers;
};
}} // namespace harmoniq::layer
