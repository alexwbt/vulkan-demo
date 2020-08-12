#include "vulkan-state.h"

namespace Vulkan
{
    CommandPool::CommandPool()
    {
        create();
        State::setCommandPool(this);
        APPLICATION_LOG("Created Command Pool.");
    }

    CommandPool::~CommandPool()
    {
        destroy();
        APPLICATION_LOG("Destroyed Command Pool.");
    }

    void CommandPool::create()
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(State::getPhysicalDevice());
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = 0;
        if (vkCreateCommandPool(State::getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create command pool.");
    }

    void CommandPool::destroy()
    {
        vkDestroyCommandPool(State::getDevice(), commandPool, nullptr);
    }

    VkCommandPool CommandPool::getCommandPool()
    {
        return commandPool;
    }
}
