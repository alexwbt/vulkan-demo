#include "configuration-macro.h"
#include "vulkan-state.h"

#include <set>
#include <vector>
#include <stdexcept>

namespace Vulkan
{
    const std::vector<const char*> requiredExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, State::surface->getSurface(), &presentSupport);
            if (presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;
        }

        return indices;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(requiredExtensions.begin(), requiredExtensions.end());
        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);

        return requiredExtensions.empty();
    }

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice)
    {
        return findQueueFamilies(physicalDevice).isComplete()
            && checkDeviceExtensionSupport(physicalDevice);
    }

    PhysicalDevice::PhysicalDevice()
    {
        physicalDevice = VK_NULL_HANDLE;

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(State::instance->getInstance(), &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("Failed to find GPU with Vulkan support.");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(State::instance->getInstance(), &deviceCount, devices.data());

        for (const auto& device : devices)
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                break;
            }

        if (physicalDevice == VK_NULL_HANDLE)
            throw std::runtime_error("Failed to find a suitable GPU.");

        State::physicalDevice = this;
        APPLICATION_LOG("Selected Physical Device.")
    }

    VkPhysicalDevice& PhysicalDevice::getPhysicalDevice()
    {
        return physicalDevice;
    }
}
