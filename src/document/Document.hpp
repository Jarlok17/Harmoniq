#pragma once

#include <memory>

#include "../config/precompiled.hpp"
#include "../history/CanvasHistoryManager.hpp"
#include "../layer/LayerManager.hpp"

namespace harmoniq { namespace document {
class Document : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
        Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modifiedChanged)
        Q_PROPERTY(layer::LayerManager *layerManager READ layerManager NOTIFY layerManagerChanged)
        Q_PROPERTY(harmoniq::CanvasHistoryManager *historyManager READ historyManager NOTIFY historyManagerChanged)
    public:
        Document(QObject *parent = nullptr);

        QString name() const { return m_name; }
        QString path() const { return m_path; }

        QJsonObject toJson() const;
        static Document *fromJson(const QJsonObject &obj, QObject *parent = nullptr);

        layer::LayerManager *layerManager() const { return m_layerManager.get(); }
        harmoniq::CanvasHistoryManager *historyManager() const { return m_historyManager.get(); }
        bool isModified() const { return m_modified; }

    public slots:
        void setName(const QString &name);
        void setPath(const QString &path);
        void setModified(bool modified);

    private slots:
        void connectLayerSignals(layer::Layer *layer);

    signals:
        void nameChanged();
        void pathChanged();
        void modifiedChanged();
        void layerManagerChanged();
        void historyManagerChanged();

    private:
        QString m_path;
        QString m_name;
        bool m_modified = false;
        std::unique_ptr<layer::LayerManager> m_layerManager;
        std::unique_ptr<harmoniq::CanvasHistoryManager> m_historyManager;
};
}} // namespace harmoniq::document
