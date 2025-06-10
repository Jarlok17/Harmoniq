#pragma once

#include "../config/precompiled.hpp"

namespace harmoniq {
class ToolSettings : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(float size READ size WRITE setSize NOTIFY sizeChanged)
        Q_PROPERTY(float hardness READ hardness WRITE setHardness NOTIFY hardnessChanged)
        Q_PROPERTY(float opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
        Q_PROPERTY(bool fill READ fill WRITE setFill NOTIFY fillChanged)

    public:
        static ToolSettings *instance();

        Q_INVOKABLE QColor color() const { return m_color; }
        Q_INVOKABLE float size() const { return m_size; }
        Q_INVOKABLE float opacity() const { return m_opacity; }
        Q_INVOKABLE float hardness() const { return m_hardness; }
        Q_INVOKABLE bool fill() const { return m_fill; }

        Q_INVOKABLE void setColor(const QColor &color)
        {
            m_color = color;
            emit colorChanged();
        }

        Q_INVOKABLE void setOpacity(const float &opacity)
        {
            if (m_opacity != opacity) {
                m_opacity = qBound(0.0f, m_opacity, 1.0f);
                emit opacityChanged();
            }
        }

        Q_INVOKABLE void setSize(const float &size)
        {
            m_size = size;
            emit sizeChanged();
        }

        Q_INVOKABLE void setHardness(const float &hardness)
        {
            if (m_hardness != hardness) {
                m_hardness = qBound(0.0f, hardness, 1.0f);
                emit hardnessChanged();
            }
        }

        Q_INVOKABLE void setFill(const bool &fill)
        {
            m_fill = fill;
            emit fillChanged();
        }

    signals:
        void colorChanged();
        void sizeChanged();
        void hardnessChanged();
        void opacityChanged();
        void fillChanged();

    private:
        ToolSettings(QObject *parent = nullptr) : QObject(parent) {}
        static ToolSettings *m_instance;
        QColor m_color = QColor("#000000");
        float m_size = 1.f;
        float m_opacity = 1.f;
        float m_hardness = 1.f;
        bool m_fill = true;
        QVector2D m_position;
};
} // namespace harmoniq
