#pragma once

#include "../config/precompiled.hpp"
#include "../layer/LayerManager.hpp"

namespace harmoniq {

class ThumbnailImageProvider : public QQuickImageProvider
{
        Q_OBJECT
    public:
        explicit ThumbnailImageProvider() : QQuickImageProvider(QQuickImageProvider::Image), m_layerManager(nullptr) {}

        void setLayerManager(harmoniq::layer::LayerManager *layerManager)
        {
            m_layerManager = layerManager;
            emit imageChanged();
        }

        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
        {
            qDebug() << "Requesting thumbnail for id:" << id;
            if (!m_layerManager) {
                qDebug() << "LayerManager is null";
                return QImage();
            }

            bool ok;
            int index = id.split("?").first().toInt(&ok);
            if (!ok || index < 0 || index >= m_layerManager->rowCount()) {
                qDebug() << "Invalid index or out of range:" << index << "Row count:" << m_layerManager->rowCount();
                return QImage();
            }

            QImage img = m_layerManager->getLayerImage(index);
            if (img.isNull()) {
                qDebug() << "Image is null for index:" << index;
                return QImage();
            }

            qDebug() << "Thumbnail image size for index:" << index << "is" << img.size();

            bool hasTransparency = false;
            for (int y = 0; y < img.height() && !hasTransparency; ++y) {
                for (int x = 0; x < img.width() && !hasTransparency; ++x) {
                    if (qAlpha(img.pixel(x, y)) < 255) {
                        hasTransparency = true;
                    }
                }
            }

            if (hasTransparency) {
                QImage withBackground(img.size(), QImage::Format_ARGB32);
                withBackground.fill(Qt::white);
                QPainter painter(&withBackground);
                painter.drawImage(0, 0, img);
                painter.end();
                img = withBackground;
            }

            QSize targetSize(40, 40);
            if (requestedSize.isValid()) {
                targetSize.scale(requestedSize, Qt::KeepAspectRatio);
            }
            qDebug() << "Returning thumbnail for index:" << index << "Size:" << targetSize;
            return img.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

    signals:
        void imageChanged();

    private:
        harmoniq::layer::LayerManager *m_layerManager;
};
} // namespace harmoniq
