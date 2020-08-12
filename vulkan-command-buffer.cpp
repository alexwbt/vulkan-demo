#include "vulkan-command-buffer.h"
#include "vulkan-state.h"

namespace Vulkan
{
    CommandBuffer::CommandBuffer(int count, VkCommandBufferUsageFlags flags) : count(count)
    {
        commandBuffers.resize(count);

        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = State::getCommandPool();
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;

        allocate();
    }

    CommandBuffer::~CommandBuffer()
    {
        free();
    }

    void CommandBuffer::allocate()
    {
        if (vkAllocateCommandBuffers(State::getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers.");

        for (size_t i = 0; i < commandBuffers.size(); i++)
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Failed to begin recording command buffer.");
    }

    void CommandBuffer::free()
    {
        vkFreeCommandBuffers(State::getDevice(), State::getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }

    void CommandBuffer::reallocate()
    {
        free();
        allocate();
    }

    void CommandBuffer::wait()
    {
        vkQueueWaitIdle(State::deviceObj()->getGraphicsQueue());
    }

    void CommandBuffer::end()
    {
        for (size_t i = 0; i < commandBuffers.size(); i++)
            vkEndCommandBuffer(commandBuffers[i]);
    }

    void CommandBuffer::end(int index)
    {
        vkEndCommandBuffer(commandBuffers[index]);
    }

    void CommandBuffer::submit()
    {
        submit(-1, State::deviceObj()->getGraphicsQueue());
    }

    void CommandBuffer::submit(int index, VkQueue queue)
    {
        VkSubmitInfo submitInfo{};
        submit(index, queue, &submitInfo, VK_NULL_HANDLE);
    }

    void CommandBuffer::submit(int index, VkQueue queue, VkSubmitInfo* submitInfo, VkFence fence)
    {
        bool inBound = index >= 0 && index < commandBuffers.size();
        submitInfo->sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo->commandBufferCount = inBound ? 1 : (uint32_t)commandBuffers.size();
        submitInfo->pCommandBuffers = inBound ? &commandBuffers[index] : commandBuffers.data();

        vkQueueSubmit(queue, 1, submitInfo, fence);
    }

    int CommandBuffer::getSize()
    {
        return count;
    }

    VkCommandBuffer CommandBuffer::getBuffer(int index)
    {
        if (index < 0 || index >= commandBuffers.size())
            throw std::runtime_error("Get Command Buffer: Index Out Of Bounds.");
        return commandBuffers[index];
    }
}
