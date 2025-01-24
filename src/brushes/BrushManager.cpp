#include "BrushManager.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace harmoniq { namespace brushes {

BrushManager::BrushManager(QObject *parent) : QAbstractListModel(parent) {}

void BrushManager::addBrush(const QString &brushName)
{
    beginInsertRows(QModelIndex(), m_brushes.size(), m_brushes.size());
    BrushData newBrush;
    newBrush.brushName = brushName;
    m_brushes.append(newBrush);
    endInsertRows();
}

void BrushManager::removeBrush(int index)
{
    if (index < 0 || index >= m_brushes.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_brushes.removeAt(index);
    endRemoveRows();
}

int BrushManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_brushes.size();
}

QVariant BrushManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_brushes.size())
        return QVariant();

    const BrushData &brush = m_brushes[index.row()];

    switch (role) {
    case NameRole:
        return brush.brushName;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BrushManager::roleNames() const
{
    return {
        {NameRole, "name"},
    };
}

void BrushManager::createBrushJSON(const QString &filename, const BrushInfo &info)
{
    QJsonObject brushJson;

    QJsonObject effectsJson;
    if (info.effects.blurEffect.isEnabled()) {
        effectsJson["blurRadius"] = info.effects.blurEffect.getRadius();
        effectsJson["blurEnabled"] = info.effects.blurEffect.isEnabled();
    }
    if (info.effects.textureEffect.isEnabled()) {
        effectsJson["textureFile"] = info.effects.textureEffect.getTextureFile();
        effectsJson["textureOpacity"] = info.effects.textureEffect.getOpacity();
        effectsJson["textureEnabled"] = info.effects.textureEffect.isEnabled();
    }
    if (info.effects.glowEffect.isEnabled()) {
        effectsJson["glowRadius"] = info.effects.glowEffect.getRadius();
        effectsJson["glowColor"] = info.effects.glowEffect.getColor().name();
        effectsJson["glowEnabled"] = info.effects.glowEffect.isEnabled();
    }
    if (info.effects.noiseEffect.isEnabled()) {
        effectsJson["noiseScale"] = info.effects.noiseEffect.getScale();
        effectsJson["noiseIntensity"] = info.effects.noiseEffect.getIntensity();
        effectsJson["noiseEnabled"] = info.effects.noiseEffect.isEnabled();
    }
    if (info.effects.edgeEffect.isEnabled()) {
        effectsJson["edgeThickness"] = info.effects.edgeEffect.getThickness();
        effectsJson["edgeColor"] = info.effects.edgeEffect.getColor().name();
        effectsJson["edgeEnabled"] = info.effects.edgeEffect.isEnabled();
    }
    brushJson["effects"] = effectsJson;

    QJsonDocument jsonDoc(brushJson);
    QString filePath = QString(BRUSH_DIRECTORY) + "/" + filename;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();
    qDebug() << "Brush saved to JSON:" << filePath;
}
QVector<QString> BrushManager::getAllBrushesFromDirectory()
{
    QVector<QString> brushFiles;
    QDir dir(BRUSH_DIRECTORY);

    if (!dir.exists()) {
        qWarning() << "Brush directry does not exist:" << BRUSH_DIRECTORY;
        return brushFiles;
    }

    QStringList filters;
    filters << "*.json";
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        brushFiles.append(fileInfo.fileName());
    }

    return brushFiles;
}
}} // namespace harmoniq::brushes
