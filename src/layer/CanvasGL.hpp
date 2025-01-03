#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuickItem>
#include <QSGGeometryNode>
#include <QSGSimpleTextureNode>
#include <qopenglframebufferobject.h>

namespace harmoniq { namespace canvas {

class CanvasGL : public QQuickItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    public:
        explicit CanvasGL(QQuickItem *parent = nullptr);
        ~CanvasGL();

        void setBackgroundColor(const QColor &color);
        void setScale(const qreal &scale);
        void updateScaledSize();

        qreal scale() const { return m_scale; }
        QColor backgroundColor() const { return m_backgroundColor; }

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

    signals:
        void backgroundColorChanged();
        void scaleChanged();

    private:
        QOpenGLFramebufferObject *m_fbo = nullptr;
        QColor m_backgroundColor = QColor(255, 255, 255);
        qreal m_scale = 1.0;
        float opacity = 1.0f;
        QSize m_fboSize;
};

}} // namespace harmoniq::canvas
