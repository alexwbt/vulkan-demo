#include "vulkan-vertex-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{

    VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices): 
        vertices{ vertices },
        buffer{ sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        Buffer stagingBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        void* data;
        vkMapMemory(State::getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(State::getDevice(), stagingBuffer.getBufferMemory());

        copyBuffer(stagingBuffer.getBuffer(), buffer.getBuffer(), bufferSize);
    }

    VkBuffer VertexBuffer::getBuffer() { return buffer.getBuffer(); }
    std::vector<Vertex>& VertexBuffer::getVertices() { return vertices; }
}
