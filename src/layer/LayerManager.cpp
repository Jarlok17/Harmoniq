#include "LayerManager.hpp"

namespace harmoniq { namespace layer {

LayerManager::LayerManager(QObject *parent) : QAbstractListModel(parent) {}

int LayerManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_layers.size();
}

LayerManager::~LayerManager() {}

QVariant LayerManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_layers.size())
        return QVariant();

    const LayerData &layer = m_layers[index.row()];

    switch (role) {
    case NameRole:
        return layer.name;
    case LockedRole:
        return layer.locked;
    case WidthRole:
        return layer.layer->width();
    case HeightRole:
        return layer.layer->height();
    case ColorRole:
        return layer.layer->backgroundColor();
    case VisibleRole:
        return layer.visible;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LayerManager::roleNames() const
{
    return {
        {   NameRole,         "name"},
        { LockedRole,       "locked"},
        {  WidthRole,   "LayerWidth"},
        { HeightRole,  "LayerHeight"},
        {  ColorRole,   "LayerColor"},
        {VisibleRole, "LayerVisible"}
    };
}

void LayerManager::addLayer(const QString &lname, const int &w, const int &h, const QColor &background, const bool &locked)
{
    beginInsertRows(QModelIndex(), m_layers.size(), m_layers.size());
    LayerData newLayer;
    newLayer.name = lname;
    newLayer.locked = locked;
    newLayer.visible = true;
    newLayer.layer = std::make_shared<Layer>();
    newLayer.layer->setWidth(w);
    newLayer.layer->setVisible(newLayer.visible);
    newLayer.layer->setHeight(h);
    newLayer.layer->setBackgroundColor(background);
    m_layers.append(newLayer);
    qDebug() << "Шар додано успішно";
    endInsertRows();
}

void LayerManager::removeLayer(const int &index)
{
    if (index >= 0 && index < m_layers.size()) {
        if (!m_layers[index].locked) {
            beginRemoveRows(QModelIndex(), index, index);
            m_layers.removeAt(index);
            qDebug() << "Шар видалено успішно: " << index;
            endRemoveRows();
        }
    }
}

void LayerManager::moveLayer(const int &from, const int &to)
{
    if (from < 0 || from >= m_layers.size() || to < 0 || to >= m_layers.size() || from == to) {
        return;
    }

    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to > from ? to + 1 : to);
    m_layers.move(from, to);
    endMoveRows();
    qDebug() << "Шар переміщено з " << from << " на " << to;
}

bool LayerManager::isLayerLocked(const int &index) const
{
    if (index >= 0 && index < m_layers.size()) {
        return m_layers[index].locked;
    }
    return false;
}

void LayerManager::setLayerLocked(const int &index, const bool &locked)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].locked = locked;
        emit dataChanged(this->index(index), this->index(index), {LockedRole});
    }
}

void LayerManager::setLayerVisible(const int &index, const bool &visible)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].visible = visible;
        qDebug() << "Layer visibility changed for index " << index << ": " << visible;
        emit dataChanged(this->index(index), this->index(index), {VisibleRole});
    }
}
}} // namespace harmoniq::layer
