#include "window.h"
#include "vulkan-state.h"

#include <iostream>

using namespace Vulkan;

int main()
{
    try
    {
        Window::initialize(800, 600, "Vulkan Demo");

        Instance instance;
        Surface surface;
        PhysicalDevice physicalDevice;
        Device device;
        Swapchain swapchain;

        while (Window::update())
        {

        }

        Window::terminate();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
