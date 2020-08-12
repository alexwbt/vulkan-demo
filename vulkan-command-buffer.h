#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace Vulkan
{
    class CommandBuffer
    {
    public:
        CommandBuffer(int count, VkCommandBufferUsageFlags flags);
        ~CommandBuffer();

        void reallocate();
        void wait();
        void end();
        void end(int index);
        void submit();
        void submit(int index, VkQueue queue);
        void submit(int index, VkQueue queue, VkSubmitInfo* submitInfo, VkFence fence);

        int getSize();
        VkCommandBuffer getBuffer(int index);
    private:
        int count;
        std::vector<VkCommandBuffer> commandBuffers;

        VkCommandBufferAllocateInfo allocInfo{};
        VkCommandBufferBeginInfo beginInfo{};

        void allocate();
        void free();
    };
}
