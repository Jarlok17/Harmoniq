#pragma once

#include <GL/gl.h>
#include <QtOpenGL/QtOpenGL>

namespace harmoniq { namespace layer {
struct Layer
{
        GLuint textureID;  // OpenGL texture identifier
        float opacity;     // Layer opacity
        bool visible;      // Layer visibility
        int width, height; // Texture size

        Layer(int w, int h) : opacity(1.0f), visible(true), width(w), height(h)
        {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ~Layer() { glDeleteTextures(1, &textureID); }

        void bind() { glBindTexture(GL_TEXTURE_2D, textureID); }

        void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
};
}} // namespace harmoniq::layer
