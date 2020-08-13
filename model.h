#pragma once

#include <string>
#include <vector>

#include "vulkan-vertex-buffer.h"

class Model
{
public:
    Model(std::string path);

    std::vector<Vulkan::Vertex> vertices;
    std::vector<uint32_t> indices;
};
