#include "vulkan-vertex-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(State::getPhysicalDevice(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        throw std::runtime_error("Failed to find suitable memory type.");
    }

    VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices) : vertices{vertices}
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(State::getDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to create vertex buffer.");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(State::getDevice(), vertexBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (vkAllocateMemory(State::getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate vertex buffer memory.");

        vkBindBufferMemory(State::getDevice(), vertexBuffer, bufferMemory, 0);

        void* data;
        vkMapMemory(State::getDevice(), bufferMemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(State::getDevice(), bufferMemory);
    }

    VertexBuffer::~VertexBuffer()
    {
        vkDestroyBuffer(State::getDevice(), vertexBuffer, nullptr);
        vkFreeMemory(State::getDevice(), bufferMemory, nullptr);
    }

    VkBuffer VertexBuffer::getVertexBuffer()
    {
        return vertexBuffer;
    }

    std::vector<Vertex>& VertexBuffer::getVertices()
    {
        return vertices;
    }
}
