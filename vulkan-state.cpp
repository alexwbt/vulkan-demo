#include "vulkan-state.h"

#include <string>
#include <iostream>
#include <stdexcept>

#define STATE_GETTER_SETTER(name, member) State::get##name()\
{\
    return(##member->get##name());\
}\
void State::set##name(##name* ##member)\
{\
    if (##member != nullptr)\
        throw std::runtime_error("State shouldnt be set more than one time.");\
    State::member = member;\
}\

namespace Vulkan
{
    Instance* State::instance = nullptr;
    Surface* State::surface = nullptr;
    PhysicalDevice* State::physicalDevice = nullptr;


    VkInstance& STATE_GETTER_SETTER(Instance, instance)

    VkSurfaceKHR& State::getSurface()
    {
    }
    void State::setSurface(Surface* instance)
    {}

    VkPhysicalDevice& State::getPhysicalDevice()
    {}
    void State::setPhysicalDevice(PhysicalDevice* instance)
    {}
}
