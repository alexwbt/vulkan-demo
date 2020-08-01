#include "configuration-macro.h"

#include "vulkan-state.h"
#include "window.h"

#include <stdexcept>

namespace Vulkan
{
    Surface::Surface()
    {
        if (glfwCreateWindowSurface(State::getInstance(), Window::window, nullptr, &surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface.");

        State::setSurface(this);
        APPLICATION_LOG("Created Surface.")
    }

    Surface::~Surface()
    {
        vkDestroySurfaceKHR(State::getInstance(), surface, nullptr);

        APPLICATION_LOG("Destroyed Surface.")
    }

    VkSurfaceKHR& Surface::getSurface()
    {
        return surface;
    }
}
