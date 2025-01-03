#pragma once

#include "Vulkan.hpp";
#include <vector>

namespace Vulkan {

class WindowSystem;

class Instance final{
public:
    Instance(Instance&&) = delete;
    Instance(const WindowSystem& window);
    ~Instance();

private:
    VkInstance m_instance;
    VkInstance Handle() const {
        return m_instance;
    }

    const std::vector<const char*> m_validation_layers { "VK_LAYER_KHRONOS_validation" };
};

}
