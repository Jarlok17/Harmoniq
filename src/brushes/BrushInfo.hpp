#pragma once

#include "../configs/precompiled.hpp"

namespace harmoniq { namespace brushes {

struct Brush
{
        QColor color = Qt::black;
        float size = 1.f;
        float intensity = 1.f;
        float opacity = 1.f;
        QVector2D position;
};
}} // namespace harmoniq::brushes
