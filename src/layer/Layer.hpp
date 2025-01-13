#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq { namespace layer {

class Layer : public QQuickItem
{
        Q_OBJECT

        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    public:
        explicit Layer(QQuickItem *parent = nullptr);
        explicit Layer(const int &w, const int &h, const QColor &background, QQuickItem *parent = nullptr);
        ~Layer();

        void initializeGL(QOpenGLContext *currentContext);

        void setBackgroundColor(const QColor &color);
        void setScale(const qreal &scale);

        qreal scale() const { return m_scale; }
        QColor backgroundColor() const { return m_backgroundColor; }

    protected:
        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

    signals:
        void backgroundColorChanged();
        void scaleChanged();

    private:
        QOpenGLFramebufferObject *m_fbo = nullptr;
        QOpenGLShaderProgram *m_shaderProgram = nullptr;

        QColor m_backgroundColor = QColor(255, 255, 255);
        qreal m_scale = 1.0;
        QSize m_fboSize;

        GLuint m_vao = 0;
        GLuint m_vbo = 0;
        bool m_geometryInitialized = false;
};

}} // namespace harmoniq::layer
