#pragma once

#include "vulkan-buffer.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>

namespace Vulkan
{
    struct UniformBufferObject {
        glm::mat4 pvm;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
    };

    class DescriptorSet
    {
    public:
        DescriptorSet();
        ~DescriptorSet();

        void createDescriptorPool();
        void createDescriptorSets(std::vector<Buffer*>& uniformBuffers);
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
