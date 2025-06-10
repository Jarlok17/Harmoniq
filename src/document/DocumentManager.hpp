#pragma once

#include "Document.hpp"

namespace harmoniq { namespace document {

class DocumentManager : public QAbstractListModel
{
        Q_OBJECT
        Q_PROPERTY(Document *current READ current WRITE setCurrent NOTIFY currentChanged)
        Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentChanged)
        Q_PROPERTY(layer::LayerManager *currentLayerManager READ currentLayerManager NOTIFY layerManagerChanged)

    public:
        enum DocumentRoles {
            NameRole = Qt::UserRole + 100,
            PathRole,
        };

        explicit DocumentManager(QObject *parent = nullptr);

        Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const override;
        Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void addDocument(const QString &name, const QString &path);
        Q_INVOKABLE void removeDocument(int index);
        Q_INVOKABLE bool saveToFile(const QString &filepath);
        Q_INVOKABLE void loadFromFile(const QString &filepath);
        Q_INVOKABLE bool exportImage(const QString &filepath);
        Q_INVOKABLE void setCurrentIndex(int index);
        Q_INVOKABLE Document *get(int index) const;
        Q_INVOKABLE int currentIndex() const;

        Q_INVOKABLE Document *current() const;
        Q_INVOKABLE void setCurrent(Document *doc);
        Q_INVOKABLE layer::LayerManager *currentLayerManager() const;
        Q_INVOKABLE int count() const { return m_documents.count(); }

    signals:
        void documentsChanged();
        void currentChanged();
        void layerManagerChanged();

    private:
        QList<Document *> m_documents;
        Document *m_currentDocument;
};

}} // namespace harmoniq::document
