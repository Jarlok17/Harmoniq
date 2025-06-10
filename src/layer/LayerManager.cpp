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

    const auto &layer = m_layers.at(index.row());

    switch (role) {
    case NameRole:
        return layer.name;
    case LockedRole:
        return layer.locked;
    case VisibleRole:
        return layer.visible;
    case WidthRole:
        return layer.layer->width();
    case HeightRole:
        return layer.layer->height();
    case ColorRole:
        return layer.layer->backgroundColor();
    case LayerObjectRole:
        return QVariant::fromValue(layer.layer);
    default:
        return QVariant();
    }
}

Layer *LayerManager::getCurrentLayer() const
{
    if (m_currentIndex < 0 || m_currentIndex >= m_layers.size())
        return nullptr;
    return m_layers[m_currentIndex].layer;
}

LayerManager::LayerData *LayerManager::get(int index)
{
    if (index < 0 || index >= m_layers.size())
        return nullptr;
    return &m_layers[index];
}

QImage LayerManager::getMergedImage() const
{
    if (m_layers.isEmpty()) {
        return QImage();
    }

    QSize canvasSize = layerSize;
    QImage finalImage(canvasSize, QImage::Format_ARGB32);
    finalImage.fill(Qt::transparent);

    QPainter painter(&finalImage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    for (const auto &layerData : m_layers) {
        if (layerData.visible) {
            QImage layerImage = layerData.layer->image();
            if (!layerImage.isNull()) {
                painter.drawImage(0, 0, layerImage);
            }
        }
    }

    painter.end();
    return finalImage;
}

QHash<int, QByteArray> LayerManager::roleNames() const
{
    return {
        {       NameRole,        "name"},
        {     LockedRole,      "locked"},
        {    VisibleRole,     "visible"},
        {      WidthRole,       "width"},
        {     HeightRole,      "height"},
        {      ColorRole,       "color"},
        {LayerObjectRole, "layerObject"}
    };
}

void LayerManager::addLayer(const QString &lname, const int &w, const int &h, const QColor &background, const bool &locked)
{
    int widthToUse;
    int heightToUse;
    if (m_layers.isEmpty()) {
        layerSize.setWidth(w);
        layerSize.setHeight(h);
        widthToUse = w;
        heightToUse = h;
    } else {
        widthToUse = layerSize.width();
        heightToUse = layerSize.height();
    }

    beginInsertRows(QModelIndex(), m_layers.size(), m_layers.size());
    LayerData newLayer;
    newLayer.name = lname;
    newLayer.locked = locked;
    newLayer.visible = true;

    newLayer.layer = new Layer();
    newLayer.layer->setWidth(widthToUse);
    newLayer.layer->setHeight(heightToUse);
    newLayer.layer->setBackgroundColor(background);
    newLayer.layer->updateCanvasSize();

    newLayer.layer->setCurrentTool(m_currentTool);

    m_layers.append(newLayer);
    qDebug() << "LAYER COUNT" << this->rowCount();
    endInsertRows();

    emit layerAdded(newLayer.layer);

    setCurrentIndex(m_layers.count() - 1);
}

void LayerManager::setLayerName(const int &index, const QString &name)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].name = name;
        emit dataChanged(this->index(index), this->index(index), {NameRole});
        qDebug() << "Layer name changed to:" << name << "at index:" << index;
    }
}

void LayerManager::removeLayer(const int &index)
{
    if (index >= 0 && index < m_layers.size()) {
        if (!m_layers[index].locked) {
            beginRemoveRows(QModelIndex(), index, index);
            delete m_layers[index].layer;
            m_layers.removeAt(index);
            qDebug() << "Шар видалено успішно: " << index;
            endRemoveRows();
        }
        setCurrentIndex(m_layers.count() - 1);
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

void LayerManager::setCurrentIndex(int index)
{
    if (index >= 0 && index < m_layers.size() && m_currentIndex != index) {
        m_currentIndex = index;
        emit currentIndexChanged();
        qDebug() << "CURRENT INDEX: " << m_currentIndex;
    }

    for (int i = 0; i < m_layers.size(); i++) {
        bool active = (i == m_currentIndex);
        if (m_layers[i].layer) {
            m_layers[i].layer->setActive(active);

            if (active) {
                m_layers[i].layer->setCurrentTool(m_currentTool);
            }
        }
    }
}

bool LayerManager::isLayerLocked(const int &index) const
{
    if (index >= 0 && index < m_layers.size()) {
        return m_layers[index].layer->isLocked();
    }
    return false;
}

void LayerManager::setCurrentTool(ToolType tool)
{
    if (m_currentTool != tool) {
        m_currentTool = tool;

        if (m_currentIndex >= 0 && m_currentIndex < m_layers.size()) {
            auto &layerData = m_layers[m_currentIndex];
            if (layerData.layer) {
                layerData.layer->setCurrentTool(tool);
            }
        }
        emit toolChanged(tool);
    }
}

void LayerManager::setLayerLocked(const int &index, const bool &locked)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].locked = locked;
        m_layers[index].layer->setLocked(locked);
        emit dataChanged(this->index(index), this->index(index), {LockedRole});
    }
}

void LayerManager::setLayerVisible(const int &index, const bool &visible)
{
    if (index >= 0 && index < m_layers.size()) {
        m_layers[index].visible = visible;
        m_layers[index].layer->setVisibility(visible);
        emit dataChanged(this->index(index), this->index(index), {VisibleRole});
    }
}
}} // namespace harmoniq::layer
