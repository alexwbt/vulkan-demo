#include "configuration-macro.h"

#include "vulkan-state.h"
#include "window.h"

#include <stdexcept>

namespace Vulkan
{
    Surface::Surface()
    {
        if (glfwCreateWindowSurface(State::instance->getInstance(), Window::window, nullptr, &surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface.");

        State::surface = this;
        APPLICATION_LOG("Created Surface.")
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(State::instance->getInstance(), surface, nullptr);

        APPLICATION_LOG("Destroyed Surface.")
    }

    VkSurfaceKHR& Surface::getSurface()
    {
        return surface;
    }
}
