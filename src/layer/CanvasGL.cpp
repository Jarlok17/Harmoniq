#include "CanvasGL.hpp"
#include <QFile>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QThread>
#include <iostream>

namespace harmoniq { namespace canvas {

CanvasGL::CanvasGL(QQuickItem *parent) : QQuickItem(parent) { setFlag(ItemHasContents, true); }

CanvasGL::CanvasGL(const int &w, const int &h, const QColor &background, QQuickItem *parent)
    : QQuickItem(parent)
    , m_fboSize(w, h)
    , m_backgroundColor(background)
{
    setFlag(ItemHasContents, true);
}

CanvasGL::~CanvasGL()
{
    auto *context = QOpenGLContext::currentContext();
    if (context) {
        auto *glFuncs = context->extraFunctions();
        if (m_geometryInitialized) {
            if (m_vao) {
                glFuncs->glDeleteVertexArrays(1, &m_vao);
                m_vao = 0;
            }
            if (m_vbo) {
                glFuncs->glDeleteBuffers(1, &m_vbo);
                m_vbo = 0;
            }
        }
    }

    if (m_fbo) {
        delete m_fbo;
        m_fbo = nullptr;
    }
    if (m_shaderProgram) {
        delete m_shaderProgram;
        m_shaderProgram = nullptr;
    }
}

void CanvasGL::initializeGL(QOpenGLContext *currentContext)
{
    m_shaderProgram = new QOpenGLShaderProgram();

    if (!currentContext) {
        qWarning() << "No OpenGL context is current in this thread!";
        return;
    }

    auto *glFuncs = currentContext->extraFunctions();

    QFile vertexShaderFile(":/shaders/canvas.vert");
    if (!vertexShaderFile.exists()) {
        qWarning() << "Vertex shader file does not exist: :/shaders/canvas.vert";
    }

    QFile fragmentShaderFile(":/shaders/canvas.frag");
    if (!fragmentShaderFile.exists()) {
        qWarning() << "Fragment shader file does not exist: :/shaders/canvas.frag";
    }

    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/canvas.vert")) {
        qWarning() << "Failed to add vertex shader: " << m_shaderProgram->log();
    }
    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/canvas.frag")) {
        qWarning() << "Failed to add fragment shader: " << m_shaderProgram->log();
    }

    if (!m_shaderProgram->link()) {
        qWarning() << "Failed to link shader program: " << m_shaderProgram->log();
    }

    glFuncs->glGenVertexArrays(1, &m_vao);
    glFuncs->glGenBuffers(1, &m_vbo);

    GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

    glFuncs->glBindVertexArray(m_vao);

    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glFuncs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);

    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFuncs->glBindVertexArray(0);

    m_geometryInitialized = true;
}

QSGNode *CanvasGL::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    auto *context = QOpenGLContext::currentContext();
    auto *glFuncs = context->extraFunctions();
    if (!glFuncs) {
        qWarning() << "OpenGL context is not valid!";
        return nullptr;
    }

    if (!m_geometryInitialized) {
        initializeGL(context);
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

        glFuncs->glEnable(GL_BLEND);
        glFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glFuncs->glViewport(0, 0, m_fboSize.width() * m_scale, m_fboSize.height() * m_scale);
        glFuncs->glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(),
                              QQuickItem::opacity());
        glFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFuncs->glDisable(GL_SCISSOR_TEST);

        if (!m_shaderProgram || !m_shaderProgram->bind()) {
            qWarning() << "Shader program is not valid!";
            return nullptr;
        } else {
            float opacity = QQuickItem::opacity();
            m_shaderProgram->setUniformValue("opacity", opacity);
            m_shaderProgram->setUniformValue(
                "color", QVector3D(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF()));

            glFuncs->glBindVertexArray(m_vao);
            glFuncs->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            m_shaderProgram->release();
        }

        m_fbo->release();

        auto texture = QNativeInterface::QSGOpenGLTexture::fromNative(m_fbo->texture(), window(), m_fboSize);
        textureNode->setFiltering(QSGTexture::Linear);
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

void CanvasGL::setScale(const qreal &scale)
{
    const qreal maxScale = 3.0;
    const qreal minScale = 0.3;

    qreal clampedScale = std::clamp(scale, minScale, maxScale);

    std::cout << "Requested Scale: " << scale << ", Clamped Scale: " << clampedScale << std::endl;

    if (!qFuzzyCompare(m_scale, clampedScale)) {
        m_scale = clampedScale;

        m_fboSize = QSize(m_fboSize.width() * m_scale, m_fboSize.height() * m_scale);

        emit scaleChanged();
        update();
    }
}

}} // namespace harmoniq::canvas
