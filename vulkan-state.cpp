#include "vulkan-state.h"

#define STATE_GETTER_SETTER(name, member, returnType) name* State::member = nullptr;\
returnType& State::get##name()\
{\
    if (##member == nullptr)\
        throw std::runtime_error("Failed to get state." #member " is undefined.");\
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

    VkQueue* State::getGraphicsQueue()
    {
        return &graphicsQueue;
    }

    VkQueue* State::getPresentQueue()
    {
        return &presentQueue;
    }

    QueueFamilyIndices State::findQueueFamilies(VkPhysicalDevice physicalDevice)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, State::getSurface(), &presentSupport);
            if (presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
        }

        return indices;
    }
}
