#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq { namespace layer {
class LayerRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLExtraFunctions
{
    public:
        LayerRenderer();
        ~LayerRenderer() override;

        void render() override;
        QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

        void setBackgroundColor(const QColor &color);
        void setVisible(bool visible) { m_visible = visible; }

    private:
        QOpenGLShaderProgram m_program;
        GLuint m_vbo;

        QColor m_backgroundColor;
        bool m_visible = true;
};
}} // namespace harmoniq::layer
