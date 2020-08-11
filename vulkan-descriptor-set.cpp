#include "vulkan-state.h"

namespace Vulkan
{
    DescriptorSet::DescriptorSet()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(State::getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create descriptor set layout.");

        State::setDescriptorSet(this);
        APPLICATION_LOG("Created Descriptor Set.");
    }

    DescriptorSet::~DescriptorSet()
    {
        destroyDescriptorPool();
        vkDestroyDescriptorSetLayout(State::getDevice(), descriptorSetLayout, nullptr);
        APPLICATION_LOG("Destroyed Descriptor Set.");
    }

    void DescriptorSet::createDescriptorPool()
    {
        uint32_t imageCount = static_cast<uint32_t>(State::swapchainObj()->getImageCount());

        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = imageCount;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = imageCount;

        if (vkCreateDescriptorPool(State::getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create descriptor pool.");
    }

    void DescriptorSet::createDescriptorSets(std::vector<Buffer*>& uniformBuffers)
    {
        std::vector<VkDescriptorSetLayout> layouts(State::swapchainObj()->getImageCount(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(State::swapchainObj()->getImageCount());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(State::swapchainObj()->getImageCount());
        if (vkAllocateDescriptorSets(State::getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate descriptor sets.");

        for (size_t i = 0; i < descriptorSets.size(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i]->getBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(State::getDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }

    void DescriptorSet::destroyDescriptorPool()
    {
        vkDestroyDescriptorPool(State::getDevice(), descriptorPool, nullptr);
    }

    VkDescriptorSetLayout* DescriptorSet::getDescriptorSetLayoutPointer() { return &descriptorSetLayout; }
    VkDescriptorPool DescriptorSet::getDescriptorPool() { return descriptorPool; }
    std::vector<VkDescriptorSet>& DescriptorSet::getDescriptorSets() { return descriptorSets; }
}
