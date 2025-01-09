#pragma once

#include "Vulkan.hpp";
#include "Vulkan.hpp"

namespace Vulkan {

class Window;

class Instance final {
public:
    Instance(Instance&&) = delete;
    Instance(const Window& window);
    ~Instance();

    auto        GetHandle() const { return m_instance; }
    const auto  GetInstanceExtensions() const { return m_instance_extensions; }
    const auto  GetPhysicalDevices() const { return m_physical_devices; }
    const auto  GetLayers() const { return m_layers; }
    const auto& GetWindow() const { return m_window; }

private:
    uint32_t   m_vulkan_api_version { VK_API_VERSION_1_0 };
    VkInstance m_instance { nullptr };

    const Window& m_window;

    std::vector<VkPhysicalDevice>  m_physical_devices;
    std::vector<VkLayerProperties> m_layers;
    std::vector<const char*>       m_instance_extensions;

    const std::vector<const char*> m_validation_layers { "VK_LAYER_KHRONOS_validation" };

    void CheckValidationLayers(const std::vector<VkLayerProperties>& layers) const;
};

} // namespace Vulkan
