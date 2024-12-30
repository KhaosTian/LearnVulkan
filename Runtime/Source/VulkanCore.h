#pragma once

#include "WindowSystem.h"
#include "VulkanTypes.h"
#include "VulkanMacro.h"
#include "VulkanIncludes.h"

namespace Nova {

struct VulkanCoreInitInfo {
    std::shared_ptr<WindowSystem> window_system;
};

class VulkanCore {
private:
    GLFWwindow* m_window { nullptr };
    VkInstance  m_instance { nullptr };

    std::vector<const char*> m_validation_layers { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> m_instance_extensions {};

    VkDebugUtilsMessengerEXT m_debug_messenger { nullptr };
    std::vector<const char*> m_device_extensions { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkSurfaceKHR m_surface { nullptr };

    VkPhysicalDevice m_physical_device { nullptr };
    VkDevice         m_device { nullptr };

    VkQueue m_graphics_queue { nullptr };
    VkQueue m_present_queue { nullptr };
    VkQueue m_compute_queue { nullptr };

    QueueFamilyIndices m_queue_indices {};

    VkSwapchainKHR       m_swapchain {};
    std::vector<VkImage> m_swapchain_images;

    bool     m_enable_validation_layers { true };
    uint32_t m_vulkan_api_version { VK_API_VERSION_1_0 };

private:
public:
    void Init(VulkanCoreInitInfo init_info);
    void CreateInstance();
    void InitDebugMessenger();
    void CreateWindowSurface();
    void InitPhysicalDevice();
    void CreateLogicalDevice();

private:
    bool                     CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions() const;

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

    bool               IsDeviceSuitable(VkPhysicalDevice physical_device);
    QueueFamilyIndices FindQueueFamiliyIndices(VkPhysicalDevice physical_device) const;
    bool               CheckDeviceExtensionSupport(VkPhysicalDevice physical_device);

    void DestroyDebugUtilsMessengerEXT();
};
} // namespace Nova