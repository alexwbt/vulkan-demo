#pragma once

#include <vulkan/vulkan.h>

namespace Vulkan
{
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    class Buffer
    {
    public:
        Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
        ~Buffer();

        void storeData(void* data);
        void copy(Buffer& buffer);

        VkDeviceSize getSize();
        VkBuffer getBuffer();
    private:
        VkDeviceSize size;
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
    };
}
