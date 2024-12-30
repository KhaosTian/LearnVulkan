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
void VulkanCore::CreateInstance() {
    if (m_enable_validation_layers && !CheckValidationLayerSupport()) {
        LOG_ERROR("Failed to get available validation layers!");
    }

    m_vulkan_api_version = VK_API_VERSION_1_0;

    // app info
    VkApplicationInfo app_info  = {};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = "Nova Renderer";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName        = "Nova";
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = m_vulkan_api_version;

    // create info
    VkInstanceCreateInfo instance_create_info = {};
    instance_create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo     = &app_info;

    std::vector<const char*> extensions          = GetRequiredExtensions();
    instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    if (m_enable_validation_layers) {
        instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};

        instance_create_info.pNext = &debug_create_info;

    } else {
        instance_create_info.enabledLayerCount   = 0;
        instance_create_info.ppEnabledLayerNames = nullptr;
    }

    if (VK_SUCCESS != vkCreateInstance(&instance_create_info, nullptr, &m_instance)) {
        LOG_ERROR("Failed to create instance!");
    }
}

bool VulkanCore::CheckValidationLayerSupport() {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_Layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_Layers.data());

    // check layer
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

    VkDebugUtilsMessengerCreateInfoEXT create_info = {};
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

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

    // determine suitable physical device
    std::map<VkPhysicalDevice, int> candidates;
    for (const auto& device: devices) {
        if (IsDeviceSuitable(device)) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            candidates.insert(std::make_pair(device, static_cast<int>(properties.deviceType)));
        }
    }

    if (candidates.empty()) {
        LOG_ERROR("Failed to find suitable physical device!");
        return;
    }

    m_physical_device = candidates.rbegin()->first;
}

bool VulkanCore::IsDeviceSuitable(VkPhysicalDevice physical_device) {
    auto queue_indeces        = FindQueueFamiliyIndices(physical_device);
    bool is_extension_support = CheckDeviceExtensionSupport(physical_device);

    if (!queue_indeces.IsComplte()) {
        return false;
    }

    return true;
}

QueueFamilyIndices VulkanCore::FindQueueFamiliyIndices(VkPhysicalDevice physical_device) const {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    // match queue family indices
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

bool VulkanCore::CheckDeviceExtensionSupport(VkPhysicalDevice physical_device) {
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

    // ensure that required extensions are supported
    std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());
    for (const auto& extension: available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

void VulkanCore::CreateLogicalDevice() {
    m_queue_indices = FindQueueFamiliyIndices(m_physical_device);

    // init device queue create info
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(3);
    InitQueueCreateInfo(queue_create_infos[0], m_queue_indices.graphics.value());
    InitQueueCreateInfo(queue_create_infos[1], m_queue_indices.present.value());
    InitQueueCreateInfo(queue_create_infos[2], m_queue_indices.compute.value());

    VkPhysicalDeviceFeatures physical_device_features = {};
    vkGetPhysicalDeviceFeatures(m_physical_device, &physical_device_features);

    VkDeviceCreateInfo device_create_info      = {};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.queueCreateInfoCount    = static_cast<uint32_t>(queue_create_infos.size());
    device_create_info.pEnabledFeatures        = &physical_device_features;
    device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
    device_create_info.enabledExtensionCount   = static_cast<uint32_t>(m_device_extensions.size());
    device_create_info.enabledLayerCount       = 0;

    if (VK_SUCCESS != vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device)) {
        LOG_ERROR("Failed to create logical device!");
    }

    // init queues of this device
    vkGetDeviceQueue(m_device, m_queue_indices.graphics.value(), 0, &m_queue_graphics);
    vkGetDeviceQueue(m_device, m_queue_indices.graphics.value(), 0, &m_queue_present);
    vkGetDeviceQueue(m_device, m_queue_indices.graphics.value(), 0, &m_queue_compute);
}

void VulkanCore::InitQueueCreateInfo(VkDeviceQueueCreateInfo& create_info, uint32_t queue_family_index) {
    float queue_priority         = 1.0f;
    create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    create_info.queueCount       = 1;
    create_info.pQueuePriorities = &queue_priority;
    create_info.queueFamilyIndex = queue_family_index;
}

void VulkanCore::CreateSwapchain() {}

void VulkanCore::CreateSwapchainImageViews() {}

} // namespace Nova