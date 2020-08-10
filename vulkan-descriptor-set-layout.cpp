// #include "vulkan-state.h"
// 
// namespace Vulkan
// {
//     DescriptorSetLayout::DescriptorSetLayout()
//     {
//         VkDescriptorSetLayoutBinding uboLayoutBinding{};
//         uboLayoutBinding.binding = 0;
//         uboLayoutBinding.descriptorCount = 1;
//         uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//         uboLayoutBinding.pImmutableSamplers = nullptr;
//         uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
// 
//         VkDescriptorSetLayoutCreateInfo layoutInfo{};
//         layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//         layoutInfo.bindingCount = 1;
//         layoutInfo.pBindings = &uboLayoutBinding;
// 
//         if (vkCreateDescriptorSetLayout(State::getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
//             throw std::runtime_error("Failed to create descriptor set layout.");
// 
//         State::setDescriptorSetLayout(this);
//         APPLICATION_LOG("Created Descriptor Set Layout.");
//     }
// 
//     DescriptorSetLayout::~DescriptorSetLayout()
//     {
//         vkDestroyDescriptorSetLayout(State::getDevice(), descriptorSetLayout, nullptr);
//         APPLICATION_LOG("Destroyed Descriptor Set Layout.");
//     }
// 
//     VkDescriptorSetLayout DescriptorSetLayout::getDescriptorSetLayout()
//     {
//         return descriptorSetLayout;
//     }
// 
//     VkDescriptorSetLayout* DescriptorSetLayout::getDescriptorSetLayoutPointer()
//     {
//         return &descriptorSetLayout;
//     }
// }
