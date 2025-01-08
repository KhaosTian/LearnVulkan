#pragma once

#include "Vulkan.hpp";
#include <vector>

namespace Vulkan {

class Window;

class Instance final {
public:
    Instance(Instance&&) = delete;
    Instance(const Window& window);
    ~Instance();

    // getter and setter
    VkInstance                           GetHandle() const { return m_instance; }
    const std::vector<VkPhysicalDevice>  GetPhysicalDevices() const { return m_physical_devices; }
    const std::vector<VkLayerProperties> GetLayers() const { return m_layers; }
    const Window&                        GetWindow() const{ return m_window; }


private:
    uint32_t                       m_vulkan_api_version { VK_API_VERSION_1_0 };
    VkInstance                     m_instance { nullptr };
    std::vector<VkPhysicalDevice>  m_physical_devices;
    std::vector<VkLayerProperties> m_layers;
    const Window&                  m_window;

    const std::vector<const char*> m_validation_layers { "VK_LAYER_KHRONOS_validation" };
    void                           CheckValidationLayerSupport(const std::vector<VkLayerProperties>& layers);
};

} // namespace Vulkan
