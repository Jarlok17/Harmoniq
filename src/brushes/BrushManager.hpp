#pragma once

#include "BrushInfo.hpp"

#define BRUSH_DIRECTORY "../../brush"

namespace harmoniq { namespace brushes {
class BrushManager : public QAbstractListModel
{
    public:
        explicit BrushManager(QObject *parent = nullptr);

        enum BrushRole {
            NameRole = Qt::UserRole + 1,
        };

        Q_INVOKABLE void addBrush(const QString &brushName);
        Q_INVOKABLE void removeBrush(int index);

        Q_INVOKABLE void createBrushJSON(const QString &filename, const BrushInfo &info);
        Q_INVOKABLE QVector<QString> getAllBrushesFromDirectory();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

    private:
        struct BrushData
        {
                QString brushName;
                BrushInfo info;
        };
        QVector<BrushData> m_brushes;
};
}} // namespace harmoniq::brushes
