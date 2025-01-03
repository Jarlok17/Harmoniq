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

    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    if (!glFuncs) {
        qWarning() << "OpenGL context is not valid!";
        return nullptr;
    }

    GLint maxTextureSize;
    glFuncs->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    QSize targetSize = size().toSize();
    if (targetSize.width() > maxTextureSize || targetSize.height() > maxTextureSize) {
        qWarning() << "FBO size exceeds maximum texture size:" << maxTextureSize;
        targetSize = targetSize.boundedTo(QSize(maxTextureSize, maxTextureSize));
    }

    if (!m_fbo || m_fboSize != targetSize) {
        if (m_fbo) {
            delete m_fbo;
        }
        m_fboSize = targetSize;
        m_fbo = new QOpenGLFramebufferObject(m_fboSize, QOpenGLFramebufferObject::CombinedDepthStencil);
    }

    if (!node) {
        node = new QSGSimpleTextureNode();
    }

    auto *textureNode = static_cast<QSGSimpleTextureNode *>(node);

    if (window()) {
        m_fbo->bind();

        glFuncs->glEnable(GL_SCISSOR_TEST);
        glFuncs->glScissor(0, 0, m_fboSize.width(), m_fboSize.height());
        glFuncs->glViewport(0, 0, m_fboSize.width() * m_scale, m_fboSize.height() * m_scale);
        glFuncs->glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(),
                              m_backgroundColor.alphaF());
        glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFuncs->glDisable(GL_SCISSOR_TEST);

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

void CanvasGL::updateScaledSize() {}

void CanvasGL::setScale(const qreal &scale)
{
    const qreal maxScale = 3.0;
    const qreal minScale = 0.3;

    qreal clampedScale = std::clamp(scale, minScale, maxScale);

    std::cout << "Requested Scale: " << scale << ", Clamped Scale: " << clampedScale << std::endl;

    if (!qFuzzyCompare(m_scale, clampedScale)) {
        m_scale = clampedScale;
        emit scaleChanged();
        update();
    }
}

}} // namespace harmoniq::canvas
