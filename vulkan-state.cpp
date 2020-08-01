#include "vulkan-state.h"

#include <string>
#include <iostream>
#include <stdexcept>

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
    STATE_GETTER_SETTER(Instance, instance, VkInstance)
    STATE_GETTER_SETTER(Surface, surface, VkSurfaceKHR)
    STATE_GETTER_SETTER(PhysicalDevice, physicalDevice, VkPhysicalDevice)
}
