#include "Document.hpp"

namespace harmoniq { namespace document {
Document::Document(QObject *parent)
    : QObject(parent)
    , m_layerManager(std::make_unique<layer::LayerManager>(this))
    , m_historyManager(std::make_unique<CanvasHistoryManager>(this))
{
    connect(m_layerManager.get(), &layer::LayerManager::layerAdded, this, &Document::connectLayerSignals);
    connect(m_historyManager.get(), &CanvasHistoryManager::stateChanged, this, [this]() { setModified(true); });

    emit layerManagerChanged();
    emit historyManagerChanged();
}

QJsonObject Document::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    obj["path"] = m_path;

    QJsonArray layersArray;
    for (int i = 0; i < m_layerManager->rowCount(); ++i) {
        auto layer = m_layerManager->get(i);

        if (layer) {
            QJsonObject layerObj;
            layerObj["name"] = layer->name;
            layerObj["locked"] = layer->locked;
            layerObj["visible"] = layer->visible;
            layerObj["width"] = layer->layer->width();
            layerObj["height"] = layer->layer->height();
            layerObj["color"] = layer->layer->backgroundColor().name();

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            layer->layer->image().save(&buffer, "PNG");
            layerObj["image"] = QString::fromLatin1(byteArray.toBase64());

            layersArray.append(layerObj);
        }
    }

    obj["layers"] = layersArray;
    return obj;
}

Document *Document::fromJson(const QJsonObject &obj, QObject *parent)
{
    auto *doc = new Document();
    doc->setName(obj["name"].toString());
    doc->setPath(obj["path"].toString());

    QJsonArray layersArray = obj["layers"].toArray();
    for (const auto &layerValue : layersArray) {
        QJsonObject layerObj = layerValue.toObject();
        QString name = layerObj["name"].toString();
        int width = layerObj["width"].toInt();
        int height = layerObj["height"].toInt();
        QColor color(layerObj["color"].toString());
        bool locked = layerObj["locked"].toBool();
        bool visible = layerObj["visible"].toBool();

        doc->layerManager()->addLayer(name, width, height, color, locked);

        int lastIndex = doc->layerManager()->rowCount() - 1;
        auto *layerWrapper = doc->layerManager()->get(lastIndex);
        if (!layerWrapper) {
            qWarning() << "Не вдалося отримати шар після додавання:" << name;
            continue;
        } else {
            qDebug() << "ПОЛОТНО " << name << " ОТРИМАННО";
        }

        layerWrapper->visible = visible;

        QString imageBase64 = layerObj["image"].toString();
        QByteArray imageData = QByteArray::fromBase64(imageBase64.toLatin1());
        QImage image;
        image.loadFromData(imageData, "PNG");

        if (!image.isNull()) {
            layerWrapper->layer->setImage(image);
            qDebug() << "ПОЛОТНО СТВОРЕНЕ З ПОСИЛАННЯМ" << image;
        } else {
            qWarning() << "Не вдалося завантажити зображення для шару:" << name;
        }
    }

    doc->layerManager()->setCurrentIndex(doc->layerManager()->rowCount() - 1);
    doc->setModified(false);

    return doc;
}

void Document::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}
void Document::setPath(const QString &path)
{
    if (m_path != path) {
        m_path = path;
        emit pathChanged();
    }
}

void Document::setModified(bool modified)
{
    if (m_modified != modified) {
        m_modified = modified;
        emit modifiedChanged();
    }
}

void Document::connectLayerSignals(layer::Layer *layer)
{
    connect(layer, &layer::Layer::stateChanged, m_historyManager.get(), &CanvasHistoryManager::saveState);
}

}} // namespace harmoniq::document
