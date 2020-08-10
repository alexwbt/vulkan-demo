#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    class Buffer
    {
    public:
        Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Buffer();

        VkBuffer getBuffer();
        VkDeviceMemory getBufferMemory();
    private:
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
    };
}
