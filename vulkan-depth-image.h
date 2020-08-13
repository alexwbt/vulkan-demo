#pragma once

#include "vulkan-image.h"

namespace Vulkan
{
    class DepthImage
    {
    public:
        DepthImage();
        ~DepthImage();

        void create();
        void destroy();

        Image& getDepthImage();
    private:
        Image depthImage;
    };
}
