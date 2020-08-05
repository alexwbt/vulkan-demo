#include "vulkan-state.h"
#include "window.h"

namespace Vulkan
{
    Surface::Surface()
    {
        if (Window::window == nullptr)
            throw std::runtime_error("Tried to create surface before window initialization.");

        if (glfwCreateWindowSurface(State::getInstance(), Window::window, nullptr, &surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface.");

        State::setSurface(this);
        APPLICATION_LOG("Created Surface.");
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(State::getInstance(), surface, nullptr);
        APPLICATION_LOG("Destroyed Surface.");
    }

    VkSurfaceKHR Surface::getSurface()
    {
        return surface;
    }
}
