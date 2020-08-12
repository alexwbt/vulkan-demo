#include "vulkan-index-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{
    IndexBuffer::IndexBuffer(std::vector<uint16_t>& indices) :
        indices{ indices },
        buffer{ sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
    {
        Buffer stagingBuffer(sizeof(indices[0])* indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.storeData(indices.data());
        buffer.copy(stagingBuffer);
    }

    VkBuffer IndexBuffer::getBuffer() { return buffer.getBuffer(); }
    std::vector<uint16_t>& IndexBuffer::getIndices() { return indices; }
}
