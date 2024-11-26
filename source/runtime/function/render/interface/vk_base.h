#pragma once
#include "vk_start.h"

namespace Vulkan
{
    class GraphicsBase
    {
        GraphicsBase() = default;
        ~GraphicsBase() = default;
        GraphicsBase(GraphicsBase&&) = delete;

    public:
        static GraphicsBase& instance()
        {
            static GraphicsBase instance;
            return instance;
        }
    };
}
