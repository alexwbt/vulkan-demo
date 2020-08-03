#include "vulkan-state.h"

#define STATE_GETTER_SETTER(name, member, returnType) name* State::member = nullptr;\
returnType& State::get##name()\
{\
    if (##member == nullptr)\
        throw std::runtime_error("Failed to get state. (" #member " is undefined)");\
    return(##member->get##name());\
}\
void State::set##name(##name* ##member)\
{\
    if (State::member != nullptr)\
        throw std::runtime_error("State shouldn't be set more than one time. (" #member ")");\
    State::member = member;\
}\

namespace Vulkan
{

    STATE_GETTER_SETTER(Instance, instance, VkInstance);
    STATE_GETTER_SETTER(Surface, surface, VkSurfaceKHR);
    STATE_GETTER_SETTER(PhysicalDevice, physicalDevice, VkPhysicalDevice);
    STATE_GETTER_SETTER(Device, device, VkDevice);
    VkQueue State::graphicsQueue;
    VkQueue State::presentQueue;
    VkQueue* State::getGraphicsQueue() { return &graphicsQueue; }
    VkQueue* State::getPresentQueue() { return &presentQueue; }
    STATE_GETTER_SETTER(Swapchain, swapchain, VkSwapchainKHR);
    VkFormat State::swapchainImageFormat;
    VkExtent2D State::swapchainExtent;
    VkFormat* State::getSwapchainImageFormat() { return &swapchainImageFormat; }
    VkExtent2D* State::getSwapchainExtent() { return &swapchainExtent; }
}
