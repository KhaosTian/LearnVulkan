#include "VulkanCore.h"
#include "VulkanExtension.h"

namespace Nova {
void VulkanCore::Init(VulkanCoreInitInfo init_info) {
    m_window   = init_info.window_system->GetWindow();
    int width  = init_info.window_system->GetWidth();
    int height = init_info.window_system->GetHeight();

#ifndef NDEBUG
    m_enable_validation_layers = true;
#else
    m_enable_validation_layers = false;
#endif

    CreateInstance();
    InitDebugMessenger();
    CreateWindowSurface();
    InitPhysicalDevice();
    CreateLogicalDevice();
}
void Nova::VulkanCore::CreateInstance() {
    if (m_enable_validation_layers && !CheckValidationLayerSupport()) {
        LOG_ERROR("Failed to get available validation layers!");
    }

    m_vulkan_api_version = VK_API_VERSION_1_0;

    // app info
    VkApplicationInfo app_info {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Nova Renderer";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "Nova";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = m_vulkan_api_version;

    // create info
    VkInstanceCreateInfo instance_create_info {};
    instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;

    std::vector<const char*> extensions          = GetRequiredExtensions();
    instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    if (m_enable_validation_layers) {
        instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};

        instance_create_info.pNext = &debug_create_info;

    } else {
        instance_create_info.enabledLayerCount   = 0;
        instance_create_info.ppEnabledLayerNames = nullptr;
    }

    if (VK_SUCCESS != vkCreateInstance(&instance_create_info, nullptr, &m_instance)) {
        LOG_ERROR("Failed to create instance");
    }
}

bool VulkanCore::CheckValidationLayerSupport() {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_Layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_Layers.data());

    for (const auto& layer_name: m_validation_layers) {
        bool found_layer = false;
        for (const auto& layer_properties: available_Layers) {
            if (std::strcmp(layer_name, layer_properties.layerName)) {
                found_layer = true;
                break;
            }
        }
    }

    return false;
}

std::vector<const char*> VulkanCore::GetRequiredExtensions() const {
    uint32_t                 extension_count     = 0;
    const char**             glfw_extensions     = glfwGetRequiredInstanceExtensions(&extension_count);
    std::vector<const char*> required_extensions = std::vector<const char*>(glfw_extensions, glfw_extensions + extension_count);

    if (m_enable_validation_layers) {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return required_extensions;
}

void VulkanCore::InitDebugMessenger() {
    if (!m_enable_validation_layers) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT create_info {};
    PopulateDebugMessengerCreateInfo(create_info);
    if (VK_SUCCESS != vkCreateDebugUtilsMessengerEXT_NOVA(m_instance, &create_info, nullptr, &m_debug_messenger)) {
        LOG_ERROR("Failed to init debug messenger!");
    }
}

void VulkanCore::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info                 = {};
    create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = vkDebugUtilsMessengerCallsbackEXT_NOVA;
}

void VulkanCore::CreateWindowSurface() {
    if (VK_SUCCESS != glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface)) {
        LOG_ERROR("Faile to create glfw window surface!");
    }
}

void VulkanCore::InitPhysicalDevice() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
    if (device_count == 0) {
        LOG_ERROR("Failed to enumerate physical devices!");
    }

    std::vector<VkPhysicalDevice> physical_devices(device_count);
    vkEnumeratePhysicalDevices(m_instance, &device_count, physical_devices.data());

    for (const auto& physical_device: physical_devices) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && IsDeviceSuitable(physical_device)) {
            m_physical_device = physical_device;
            return;
        }
    }
}

bool VulkanCore::IsDeviceSuitable(VkPhysicalDevice physical_device) {
    auto queue_indeces = FindQueueFamiliyIndices(physical_device);

    return false;
}

QueueFamilyIndices VulkanCore::FindQueueFamiliyIndices(VkPhysicalDevice physical_device) const {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    QueueFamilyIndices indices = {};
    for (int i = 0; i < queue_families.size(); i++) {
        const auto& queue_family = queue_families[i];

        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
        }

        if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            indices.compute = i;
        }

        VkBool32 is_present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, m_surface, &is_present_support);

        if (is_present_support) {
            indices.present = i;
        }

        if (indices.IsComplte()) {
            break;
        }
    }

    return indices;
}

} // namespace Nova