#include "vulkan-buffer.h"
#include "vulkan-state.h"
#include "vulkan-command-buffer.h"

#include <stdexcept>

namespace Vulkan
{
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        CommandBuffer commandBuffer(1, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer.getBuffer(0), srcBuffer, dstBuffer, 1, &copyRegion);

        commandBuffer.end();
        commandBuffer.submit();
        commandBuffer.wait();
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(State::getPhysicalDevice(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        throw std::runtime_error("Failed to find suitable memory type.");
    }

    Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : size{ size }
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(State::getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(State::getDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(State::getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate buffer memory.");

        vkBindBufferMemory(State::getDevice(), buffer, bufferMemory, 0);
    }

    Buffer::~Buffer()
    {
        vkDestroyBuffer(State::getDevice(), buffer, nullptr);
        vkFreeMemory(State::getDevice(), bufferMemory, nullptr);
    }

    void Buffer::storeData(void* data)
    {
        void* dst;
        vkMapMemory(State::getDevice(), bufferMemory, 0, size, 0, &dst);
        memcpy(dst, data, static_cast<size_t>(size));
        vkUnmapMemory(State::getDevice(), bufferMemory);
    }

    void Buffer::copy(Buffer& buffer)
    {
        if (buffer.getSize() != size)
            throw std::runtime_error("Tried to copy buffer with different size.");
        copyBuffer(buffer.getBuffer(), this->buffer, size);
    }

    VkDeviceSize Buffer::getSize() { return size; }
    VkBuffer Buffer::getBuffer() { return buffer; }
}
