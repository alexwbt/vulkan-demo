#pragma once

#include "vulkan-buffer.h"
#include "vulkan-texture.h"

#include <glm.hpp>

#include <vector>

namespace Vulkan
{
    struct UniformBufferObject {
        alignas(16) glm::mat4 pvm;
        alignas(16) glm::mat4 projection;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 model;
    };

    class DescriptorSet
    {
    public:
        DescriptorSet();
        ~DescriptorSet();

        void createDescriptorPool();
        void createDescriptorSets(std::vector<Buffer*>& uniformBuffers, Texture& texture);
        void destroyDescriptorPool();

        void getDescriptorSet() {}
        VkDescriptorSetLayout* getDescriptorSetLayoutPointer();
        VkDescriptorPool getDescriptorPool();
        std::vector<VkDescriptorSet>& getDescriptorSets();
    private:
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    };
}
