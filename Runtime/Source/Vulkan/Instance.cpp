#include "Instance.hpp"
#include "Window.hpp"
#include "Enumerate.hpp"

namespace Vulkan {
Vulkan::Instance::Instance(const Window& window): m_window(window) {
    // instance extension
    m_instance_extensions = window.GetRequiredInstanceExtensions();
    m_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    // validation layer
    m_layers = GetEnumerateVector(vkEnumerateInstanceLayerProperties);
    CheckValidationLayers(m_layers);

    //app info
    VkApplicationInfo app_info  = {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Nova Renderer";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "Nova";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = m_vulkan_api_version;

    // create info
    VkInstanceCreateInfo create_info    = {};
    create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo        = &app_info;
    create_info.enabledExtensionCount   = static_cast<uint32_t>(m_instance_extensions.size());
    create_info.ppEnabledExtensionNames = m_instance_extensions.data();
    create_info.enabledLayerCount       = static_cast<uint32_t>(m_validation_layers.size());
    create_info.ppEnabledLayerNames     = m_validation_layers.data();

    //create vulkan instance
    Check(vkCreateInstance(&create_info, nullptr, &m_instance), "create vulkan instance");

    // get instance properties
    m_physical_devices = GetEnumerateVector(m_instance, vkEnumeratePhysicalDevices);
}

Instance::~Instance() {
    if (m_instance == nullptr) return;

    vkDestroyInstance(m_instance, nullptr);
    m_instance = nullptr;
}

void Instance::CheckValidationLayers(const std::vector<VkLayerProperties>& layers) const {
    for (const auto& layer_name: m_validation_layers) {
        auto result = std::find_if(layers.begin(), layers.end(), [layer_name](const auto& layer_properties) {
            return std::strcmp(layer_name, layer_properties.layerName);
        });

        if (result == layers.end()) {
            LOG_ERROR("Counld not to find the requested validation layer");
        }
    }
}
} // namespace Vulkan
