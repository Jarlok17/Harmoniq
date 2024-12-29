#include "LayersModel.hpp"

namespace harmoniq { namespace layer {
LayersModel::LayersModel(QObject *parent) : QAbstractListModel(parent) {}

int LayersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return layers.count();
}

QVariant LayersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Layer &layer = layers[index.row()];
    switch (role) {
    case NameRole:
        return layer.name;
    case VisibilityRole:
        return layer.isVisible;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LayersModel::roleNames() const
{
    return {
        {      NameRole, "layerName"},
        {VisibilityRole, "isVisible"}
    };
}

void LayersModel::addLayer(const QString &name)
{
    beginInsertRows(QModelIndex(), layers.size(), layers.size());
    layers.append({name, true});
    endInsertRows();
}

void LayersModel::removeLayer(int index)
{
    if (index < 0 || index >= layers.size())
        return;
    beginRemoveRows(QModelIndex(), index, index);
    layers.removeAt(index);
    endRemoveRows();
}

void LayersModel::moveLayerUp(int index)
{
    if (index <= 0 || index >= layers.size())
        return;
    beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
    layers.swapItemsAt(index, index - 1);
    endMoveRows();
}

void LayersModel::moveLayerDown(int index)
{
    if (index < 0 || index >= layers.size() - 1)
        return;
    beginMoveRows(QModelIndex(), index, index, QModelIndex(), index + 2);
    layers.swapItemsAt(index, index + 1);
    endMoveRows();
}
}} // namespace harmoniq::layer
