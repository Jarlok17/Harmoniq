#pragma once

#include "BrushEffects.hpp"

namespace harmoniq { namespace brushes {

struct BrushTransform
{
        QVector2D scale;
        QMatrix2x2 rotation;
        QVector2D translation;

        float determinant() const { return rotation(0, 0) * rotation(1, 1) - rotation(0, 1) * rotation(1, 0); }
};

struct BrushInfo
{
        QColor color;
        float opacity = 1.0;
        BrushTransform transform;
        effects::BrushEffects effects;
        bool isDrawing = false;
};
}} // namespace harmoniq::brushes
