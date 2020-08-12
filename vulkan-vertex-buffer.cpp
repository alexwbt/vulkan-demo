#include "vulkan-vertex-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{

    VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices) :
        vertices{ vertices },
        buffer{ sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
    {
        Buffer stagingBuffer(sizeof(vertices[0])* vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.storeData(vertices.data());
        buffer.copy(stagingBuffer);
    }

    VkBuffer VertexBuffer::getBuffer() { return buffer.getBuffer(); }
    std::vector<Vertex>& VertexBuffer::getVertices() { return vertices; }
}
