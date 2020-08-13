#pragma once

#include "vulkan-buffer.h"

#include <vector>

namespace Vulkan
{
    class IndexBuffer
    {
    public:
        IndexBuffer(std::vector<uint32_t>& indices);

        VkBuffer getBuffer();
        std::vector<uint32_t>& getIndices();
    private:
        Buffer buffer;
        std::vector<uint32_t>& indices;
    };
}
