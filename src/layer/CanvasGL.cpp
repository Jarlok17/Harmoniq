#include "CanvasGL.hpp"
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <iostream>

namespace harmoniq { namespace canvas {

CanvasGL::CanvasGL(QQuickItem *parent) : QQuickItem(parent) { setFlag(ItemHasContents, true); }

CanvasGL::~CanvasGL() { delete m_fbo; }

QSGNode *CanvasGL::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if (!m_fbo || m_fboSize != size().toSize()) {
        delete m_fbo;
        m_fboSize = size().toSize();
        m_fbo = new QOpenGLFramebufferObject(m_fboSize, QOpenGLFramebufferObject::CombinedDepthStencil);
    }

    if (!node) {
        node = new QSGSimpleTextureNode();
    }

    auto *textureNode = static_cast<QSGSimpleTextureNode *>(node);

    if (window()) {
        m_fbo->bind();
        QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
        if (!glFuncs) {
            qWarning() << "OpenGL context is not valid!";
            return nullptr;
        }
        glFuncs->glViewport(0, 0, m_fboSize.width() * m_scale, m_fboSize.height() * m_scale);
        glFuncs->glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(),
                              m_backgroundColor.alphaF());
        glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_fbo->release();

        auto texture = QNativeInterface::QSGOpenGLTexture::fromNative(m_fbo->texture(), window(), m_fboSize);

        textureNode->setTexture(texture);
        textureNode->setRect(boundingRect());
    }

    return textureNode;
}

void CanvasGL::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void CanvasGL::updateScaledSize()
{
    if (m_scale <= 3.0f || (m_scale > 0.f && m_scale >= 0.3f)) {
        setWidth(m_fboSize.width() * m_scale);
        setHeight(m_fboSize.height() * m_scale);
    } else {
        qWarning() << "Scale value is invalid: " << m_scale;
    }
    std::cout << "Scale: " << m_scale << std::endl;
}

void CanvasGL::setScale(const qreal &scale)
{
    const qreal maxScale = 1.3;
    const qreal minScale = 0.85;

    qreal clampedScale = std::clamp(scale, minScale, maxScale);
    std::cout << "Clamped Scale: " << clampedScale << std::endl;

    if (!qFuzzyCompare(m_scale, clampedScale)) {
        m_scale = clampedScale;
        emit scaleChanged();
        updateScaledSize();
        update();
    }
}
}} // namespace harmoniq::canvas
