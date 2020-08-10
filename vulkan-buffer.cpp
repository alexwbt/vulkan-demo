#include "vulkan-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = State::getCommandPool();
        allocInfo.commandBufferCount = 1;
        
        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(State::getDevice(), &allocInfo, &commandBuffer);
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        
        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        
        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        
        vkEndCommandBuffer(commandBuffer);
        
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        
        vkQueueSubmit(State::deviceObj()->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(State::deviceObj()->getGraphicsQueue());
        
        vkFreeCommandBuffers(State::getDevice(), State::getCommandPool(), 1, &commandBuffer);
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(State::getPhysicalDevice(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        throw std::runtime_error("Failed to find suitable memory type.");
    }

    Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
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

    VkBuffer Buffer::getBuffer() { return buffer; }
    VkDeviceMemory Buffer::getBufferMemory() { return bufferMemory; }
}
