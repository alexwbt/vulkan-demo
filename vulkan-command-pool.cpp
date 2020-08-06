#include "vulkan-state.h"

namespace Vulkan
{
    void CommandPool::allocateCommandBuffers()
    {
        commandBuffers.resize(State::swapchainObj()->getImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
        if (vkAllocateCommandBuffers(State::getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffers.");

        for (size_t i = 0; i < commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                throw std::runtime_error("Failed to begin recording command buffer.");
        }
    }

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
        if (vkCreateCommandPool(State::getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool.");
        }

        allocateCommandBuffers();
    }

    void CommandPool::destroy()
    {
        vkDestroyCommandPool(State::getDevice(), commandPool, nullptr);
    }

    VkCommandPool CommandPool::getCommandPool()
    {
        return commandPool;
    }

    std::vector<VkCommandBuffer>& CommandPool::getCommandBuffers()
    {
        return commandBuffers;
    }
}
