#pragma once

#include <QObject>
#include <cstddef>
#include <memory>
#include <vector>

#include "LayerInfo.hpp"

namespace harmoniq { namespace layer {
class LayerManager
{
    public:
        std::vector<std::shared_ptr<Layer>> layers;

        void addLayer(int width, int height) { layers.push_back(std::make_shared<Layer>(width, height)); }

        void removeLayer(size_t index)
        {
            if (index < layers.size()) {
                layers.erase(layers.begin() + index);
            }
        }

        std::shared_ptr<Layer> getCurrentLayer(size_t index)
        {
            if (index < layers.size()) {
                return layers[index];
            }

            return nullptr;
        }
};
}} // namespace harmoniq::layer
