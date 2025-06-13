#include "DocumentManager.hpp"

#include "../config/precompiled.hpp"

namespace harmoniq { namespace document {

DocumentManager::DocumentManager(QObject *parent) : QAbstractListModel(parent), m_currentDocument(nullptr) {}

int DocumentManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_documents.size();
}

QVariant DocumentManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_documents.size())
        return QVariant();

    Document *doc = m_documents.at(index.row());

    switch (role) {
    case NameRole:
        return doc->name();
    case PathRole:
        return doc->path();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DocumentManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    return roles;
}

int DocumentManager::currentIndex() const { return m_documents.indexOf(m_currentDocument); }

Document *DocumentManager::get(int index) const { return m_documents.value(index, nullptr); }

layer::LayerManager *DocumentManager::currentLayerManager() const
{
    return m_currentDocument ? m_currentDocument->layerManager() : nullptr;
}

void DocumentManager::setCurrentIndex(int index)
{
    if (index >= 0 && index < m_documents.size()) {
        setCurrent(m_documents.at(index));
    }
}

bool DocumentManager::exportImage(const QString &filepath)
{
    if (!m_currentDocument) {
        qWarning() << "No current document to export.";
        return false;
    }

    QUrl url(filepath);
    if (!url.isValid()) {
        qWarning() << "Invalid file URL:" << filepath;
        return false;
    }

    QString localPath = url.toLocalFile();

#ifdef Q_OS_WIN
    if (localPath.startsWith("/") && localPath.length() > 3 && localPath[2] == ':') {
        localPath = localPath.mid(1);
    }
#endif

    QImage image = m_currentDocument->layerManager()->getMergedImage();
    if (image.isNull()) {
        qWarning() << "Failed to get merged image.";
        return false;
    }

    QFileInfo fileInfo(localPath);
    QString extension = fileInfo.suffix().toLower();
    QString format;

    if (extension == "png") {
        format = "PNG";
    } else if (extension == "jpg" || extension == "jpeg") {
        format = "JPEG";
    } else {
        qWarning() << "Unsupported image format:" << extension;
        return false;
    }

    QImageWriter writer(localPath, format.toLatin1());
    if (!writer.write(image)) {
        qWarning() << "Failed to write image to" << localPath << ":" << writer.errorString();
        return false;
    }

    qDebug() << "Successfully exported image to" << localPath;
    return true;
}

void DocumentManager::addDocument(const QString &name, const QString &path)
{
    beginInsertRows(QModelIndex(), m_documents.size(), m_documents.size());
    Document *doc = new Document();
    doc->setName(name);
    doc->setPath(path);
    m_documents.append(doc);
    endInsertRows();

    connect(doc, &Document::nameChanged, this, [this, doc]() {
        int index = m_documents.indexOf(doc);
        if (index >= 0) {
            QModelIndex modelIndex = createIndex(index, 0);
            emit dataChanged(modelIndex, modelIndex, {NameRole});
        }
    });

    if (!m_currentDocument) {
        setCurrent(doc);
    }

    emit documentsChanged();
}

void DocumentManager::removeDocument(int index)
{
    if (index < 0 || index >= m_documents.size())
        return;

    Document *doc = m_documents.at(index);

    const bool wasCurrent = (m_currentDocument == doc);

    beginRemoveRows(QModelIndex(), index, index);
    m_documents.removeAt(index);
    endRemoveRows();

    doc->deleteLater();

    if (wasCurrent) {
        m_currentDocument = nullptr;

        if (!m_documents.isEmpty()) {
            setCurrent(m_documents.first());
        } else {
            emit currentChanged();
            emit layerManagerChanged();
        }
    }

    emit documentsChanged();
}

bool DocumentManager::saveToFile(const QString &filepath)
{
    if (!m_currentDocument) {
        qWarning() << "No current document to save.";
        return false;
    }

    qDebug() << "Attempting to save to filepath:" << filepath;
    QString localPath = filepath;
    if (QUrl(filepath).isValid() && filepath.startsWith("file://")) {
        localPath = QUrl(filepath).toLocalFile();
        qDebug() << "Converted to local path:" << localPath;
    } else if (filepath.isEmpty()) {
        qWarning() << "Empty filepath provided to saveToFile";
        return false;
    }

    QFile file(localPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << localPath;
        return false;
    }

    QJsonDocument doc(m_currentDocument->toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    m_currentDocument->setPath(localPath);
    qDebug() << "Saved document to" << localPath;

    return true;
}

void DocumentManager::loadFromFile(const QString &filePath)
{
    const QString localPath = QUrl(filePath).toLocalFile();
    QFile file(localPath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        file.close();

        Document *doc = Document::fromJson(jsonDoc.object(), this);
        doc->setPath(localPath);
        beginInsertRows(QModelIndex(), m_documents.size(), m_documents.size());
        m_documents.append(doc);
        endInsertRows();

        connect(doc, &Document::nameChanged, this, [this, doc]() {
            int index = m_documents.indexOf(doc);
            if (index >= 0) {
                QModelIndex modelIndex = createIndex(index, 0);
                emit dataChanged(modelIndex, modelIndex, {NameRole});
            }
        });

        setCurrent(doc);
        emit documentsChanged();
    } else {
        qWarning() << "Failed to open file for reading:" << localPath;
    }
}

Document *DocumentManager::current() const { return m_currentDocument; }

void DocumentManager::setCurrent(Document *doc)
{
    if (m_currentDocument != doc) {
        m_currentDocument = doc;
        emit currentChanged();
        emit layerManagerChanged();
    }
}

}} // namespace harmoniq::document
