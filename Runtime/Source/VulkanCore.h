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

    VkSurfaceKHR     m_surface { nullptr };
    VkPhysicalDevice m_physical_device { nullptr };
    VkDevice         m_device { nullptr };

    // queue
    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices {};

    // swapchain
    VkSwapchainKHR       m_swapchain {};
    std::vector<VkImage> m_swapchain_images {};
    VkFormat             m_swapchain_image_format {};
    VkExtent2D           m_swapchain_extent {};

    bool     m_enable_validation_layers { true };
    uint32_t m_vulkan_api_version { VK_API_VERSION_1_0 };

public:
    void Init(VulkanCoreInitInfo init_info);
    void CreateInstance();
    void InitDebugMessenger();
    void CreateWindowSurface();
    void InitPhysicalDevice();
    void CreateLogicalDevice();

    void CreateSwapchain();
    void RecreateSwapchain();
    void CreateSwapchainImageViews();

    void Clear();

private:
    // instance
    bool                     CheckValidationLayerSupport();
    std::vector<const char*> QueryRequiredExtensions() const;

    // debug messenger
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

    // device
    bool IsDeviceSuitable(VkPhysicalDevice physical_device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice physical_device);

    // queue
    QueueFamilyIndices QueryQueueFamiliyIndices(VkPhysicalDevice physical_device) const;
    void               InitQueueCreateInfo(VkDeviceQueueCreateInfo& create_info, uint32_t queue_family_index);

    // swapchain
    SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physical_device) const;
    VkSurfaceFormatKHR      ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats);
    VkPresentModeKHR        ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& present_modes);
    VkExtent2D              ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);



};
} // namespace Nova