#pragma once

#include "../config/precompiled.hpp"

namespace harmoniq {

class ToolWrapper : public QObject
{
        Q_OBJECT
    public:
        enum class ToolType {
            None,
            Brush,
            Eraser,
            Fill,
            Move,
            Picker,
            Shape,
            Line,
            Selection
        };
        Q_ENUM(ToolType)
};

using ToolType = ToolWrapper::ToolType;

struct ToolInfo
{
        QColor color = Qt::black;
        float size = 1.0f;
        float opacity = 1.0f;
        float hardness = 1.0f;
        bool fill = true;
        QRect selection;
        QVector2D position;
};

class ToolHandler
{
    public:
        virtual ~ToolHandler() = default;
        virtual void press(QMouseEvent *event) = 0;
        virtual void move(QMouseEvent *event) = 0;
        virtual void release(QMouseEvent *event) = 0;
        virtual void setImage(QImage *image) = 0;
        virtual QImage image() const = 0;
        virtual void setToolInfo(const ToolInfo &info) = 0;
};
} // namespace harmoniq
