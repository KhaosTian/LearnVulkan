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
    CreateSwapchain();
    CreateSwapchainImageViews();
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

    std::vector<const char*> extensions          = QueryRequiredExtensions();
    instance_create_info.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
    if (m_enable_validation_layers) {
        instance_create_info.enabledLayerCount   = static_cast<uint32_t>(m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

        PopulateDebugMessengerCreateInfo(debug_create_info);
        instance_create_info.pNext = &debug_create_info;

    } else {
        instance_create_info.enabledLayerCount   = 0;
        instance_create_info.ppEnabledLayerNames = nullptr;
    }

    if (VK_SUCCESS != vkCreateInstance(&instance_create_info, nullptr, &m_instance)) {
        LOG_ERROR("Failed to create instance!");
    }
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

    // prioritize discrete GPU
    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device: devices) {
        if (IsDeviceSuitable(device)) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            int score = 0;
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                score += 1000;
            } else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                score += 100;
            }
            candidates.insert(std::make_pair(score, device));
        }
    }

    if (candidates.empty()) {
        LOG_ERROR("Failed to find suitable physical device!");
        return;
    }

    m_physical_device = candidates.rbegin()->second;
}

void VulkanCore::CreateLogicalDevice() {
    m_queue_indices = QueryQueueFamiliyIndices(m_physical_device);

    // init queue create info
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(3);
    InitQueueCreateInfo(queue_create_infos[0], m_queue_indices.graphics);
    InitQueueCreateInfo(queue_create_infos[1], m_queue_indices.present);
    InitQueueCreateInfo(queue_create_infos[2], m_queue_indices.compute);

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
    vkGetDeviceQueue(m_device, m_queue_indices.graphics, 0, &m_queue_graphics);
    vkGetDeviceQueue(m_device, m_queue_indices.graphics, 0, &m_queue_present);
    vkGetDeviceQueue(m_device, m_queue_indices.graphics, 0, &m_queue_compute);
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
        if (!found_layer) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> VulkanCore::QueryRequiredExtensions() const {
    uint32_t                 extension_count = 0;
    const char**             glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    std::vector<const char*> required_extensions =
        std::vector<const char*>(glfw_extensions, glfw_extensions + extension_count);

    if (m_enable_validation_layers) {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return required_extensions;
}

void VulkanCore::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info       = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = vkDebugUtilsMessengerCallsbackEXT_NOVA;
}

bool VulkanCore::IsDeviceSuitable(VkPhysicalDevice physical_device) {
    auto queue_indeces         = QueryQueueFamiliyIndices(physical_device);
    bool is_extension_support  = CheckDeviceExtensionSupport(physical_device);
    bool is_swapchain_adequate = false;
    if (is_extension_support) {
        auto details          = QuerySwapChainSupport(physical_device);
        is_swapchain_adequate = !details.surface_formats.empty() && !details.presentModes.empty();
    }

    VkPhysicalDeviceFeatures physical_device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

    if (!queue_indeces.IsComplete() || !is_swapchain_adequate || !physical_device_features.samplerAnisotropy) {
        return false;
    }

    return true;
}

QueueFamilyIndices VulkanCore::QueryQueueFamiliyIndices(VkPhysicalDevice physical_device) const {
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
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_surface, &is_present_support);

        if (is_present_support) {
            indices.present = i;
        }

        if (indices.IsComplete()) {
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

void VulkanCore::InitQueueCreateInfo(VkDeviceQueueCreateInfo& create_info, uint32_t queue_family_index) {
    static const float queue_priority = 1.0f;

    create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    create_info.queueCount       = 1;
    create_info.pQueuePriorities = &queue_priority;
    create_info.queueFamilyIndex = queue_family_index;
}

void VulkanCore::CreateSwapchain() {
    SwapchainSupportDetails swapchain_support_details = QuerySwapChainSupport(m_physical_device);
    VkSurfaceFormatKHR chosen_surface_format = ChooseSwapchainSurfaceFormat(swapchain_support_details.surface_formats);
    VkPresentModeKHR   chosen_present_mode   = ChooseSwapchainPresentMode(swapchain_support_details.presentModes);
    VkExtent2D         chosen_extent         = ChooseSwapchainExtent(swapchain_support_details.capabilities);

    // calculate the desired image count
    uint32_t image_count = swapchain_support_details.capabilities.minImageCount + 1;
    if (swapchain_support_details.capabilities.maxImageCount > 0 &&
        image_count > swapchain_support_details.capabilities.maxImageCount) {
        image_count = swapchain_support_details.capabilities.maxImageCount;
    }

    // swapchain create info
    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface                  = m_surface;
    create_info.minImageCount            = image_count;
    create_info.imageFormat              = chosen_surface_format.format;
    create_info.imageColorSpace          = chosen_surface_format.colorSpace;
    create_info.imageExtent              = chosen_extent;
    create_info.imageArrayLayers         = 1;
    create_info.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // concurrent mode
    if (m_queue_indices.graphics != m_queue_indices.present) {
        create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices   = m_queue_indices.data;
    }
    // exclusive mode
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform   = swapchain_support_details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode    = chosen_present_mode;
    create_info.clipped        = VK_TRUE;

    create_info.oldSwapchain = nullptr;

    if (VK_SUCCESS != vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain)) {
        LOG_ERROR("Failed to create swapchain!");
    }

    vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
    m_swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

    m_swapchain_image_format  = chosen_surface_format.format;
    m_swapchain_extent.width  = chosen_extent.width;
    m_swapchain_extent.height = chosen_extent.height;
}

SwapchainSupportDetails VulkanCore::QuerySwapChainSupport(VkPhysicalDevice physical_device) const {
    SwapchainSupportDetails details = {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &details.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, nullptr);
    if (format_count != 0) {
        details.surface_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, details.surface_formats.data());
    }

    uint32_t mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &mode_count, nullptr);
    if (mode_count != 0) {
        details.presentModes.resize(mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &mode_count, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VulkanCore::ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_fromats) {
    for (const auto& surface_format: surface_fromats) {
        if (surface_format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return surface_format;
        }
    }
    return surface_fromats.front();
}

VkPresentModeKHR VulkanCore::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& present_modes) {
    // if MAIBOX is supported, choose it
    for (const auto& present_mode: present_modes) {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return present_mode;
        }
    }
    // otherwise, choose FIFO
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanCore::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    VkExtent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

    // clamp the extent dimensions to ensure they are within the supported minimum and maximum limit
    extent.width  = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return extent;
}

void VulkanCore::RecreateSwapchain() {}

void VulkanCore::CreateSwapchainImageViews() {}

void VulkanCore::Clear() {
    if (!m_enable_validation_layers) {
        return;
    }

    vkDestroyDebugUtilsMessengerEXT_NOVA(m_instance, m_debug_messenger, nullptr);
}
} // namespace Nova