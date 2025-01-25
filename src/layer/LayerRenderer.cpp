#include "LayerRenderer.hpp"

namespace harmoniq { namespace layer {
LayerRenderer::LayerRenderer() : m_backgroundColor(255, 255, 255, 255)
{
    initializeOpenGLFunctions();

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vert/canvas.glsl")) {
        qDebug() << "Failed to load vertex shader:" << m_program.log();
    }

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag/canvas.glsl")) {
        qDebug() << "Failed to load fragment shader:" << m_program.log();
    }

    if (!m_program.link()) {
        qDebug() << "Failed to link shader program:" << m_program.log();
    } else {
        qDebug() << "Shader program linked successfully.";
    }

    static const GLfloat vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

LayerRenderer::~LayerRenderer() { glDeleteBuffers(1, &m_vbo); }

void LayerRenderer::setBackgroundColor(const QColor &color) { m_backgroundColor = color; }

void LayerRenderer::render()
{
    if (!m_visible) {
        return;
    }
    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), m_backgroundColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();

    m_program.setUniformValue("color", QVector4D(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(),
                                                 m_backgroundColor.alphaF()));

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program.disableAttributeArray(0);
    m_program.release();
    update();
}

QOpenGLFramebufferObject *LayerRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}
}} // namespace harmoniq::layer
