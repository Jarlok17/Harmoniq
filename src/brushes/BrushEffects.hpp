#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq::brushes::effects {

class EffectBase
{
    public:
        virtual ~EffectBase() = default;
        virtual bool isEnabled() const { return enabled; }

    protected:
        bool enabled = false;
};

class BlurEffect : public EffectBase
{
    public:
        void initialize(float radius)
        {
            this->radius = radius;
            enabled = true;
        }

        float getRadius() const { return radius; }

    private:
        float radius = 1.0f;
};

class TextureEffect : public EffectBase
{
    public:
        void initialize(float opacity, const QString &textureFile)
        {
            this->opacity = opacity;
            this->textureFile = textureFile;
            enabled = true;
        }

        QString getTextureFile() const { return textureFile; }
        float getOpacity() const { return opacity; }

    private:
        QString textureFile;
        float opacity = 1.0f;
};

class GlowEffect : public EffectBase
{
    public:
        void initialize(float radius, const QColor &color)
        {
            this->radius = radius;
            this->color = color;
            enabled = true;
        }

        float getRadius() const { return radius; }
        QColor getColor() const { return color; }

    private:
        float radius = 1.0f;
        QColor color = Qt::white;
};

class NoiseEffect : public EffectBase
{
    public:
        void initialize(float scale, float intensity)
        {
            this->scale = scale;
            this->intensity = intensity;
            enabled = true;
        }

        float getScale() const { return scale; }
        float getIntensity() const { return intensity; }

    private:
        float scale = 1.0f;
        float intensity = 0.5f;
};

class EdgeEffect : public EffectBase
{
    public:
        void initialize(float thickness, const QColor &color)
        {
            this->thickness = thickness;
            this->color = color;
            enabled = true;
        }

        float getThickness() const { return thickness; }
        QColor getColor() const { return color; }

    private:
        float thickness = 1.0f;
        QColor color = Qt::black;
};

struct BrushEffects
{
        BlurEffect blurEffect;
        TextureEffect textureEffect;
        GlowEffect glowEffect;
        NoiseEffect noiseEffect;
        EdgeEffect edgeEffect;
};

} // namespace harmoniq::brushes::effects
