#include <iostream>

#include "window.h"
#include "vulkan-instance.h"
#include "vulkan-surface.h"
#include "vulkan-physical-device.h"

using namespace Vulkan;

int main()
{
    try
    {
        Window::initWindow(800, 600, "Vulkan Demo");

        Instance instance;
        Surface surface;
        PhysicalDevice physicalDevice;

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
