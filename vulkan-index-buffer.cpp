#include "vulkan-index-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{
    IndexBuffer::IndexBuffer(std::vector<uint16_t>& indices) :
        indices{ indices },
        buffer{ sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        Buffer stagingBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data;
        vkMapMemory(State::getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(State::getDevice(), stagingBuffer.getBufferMemory());

        copyBuffer(stagingBuffer.getBuffer(), buffer.getBuffer(), bufferSize);
    }

    VkBuffer IndexBuffer::getBuffer() { return buffer.getBuffer(); }
    std::vector<uint16_t>& IndexBuffer::getIndices() { return indices; }
}
