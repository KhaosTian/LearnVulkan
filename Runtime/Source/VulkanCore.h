#pragma once

#include "WindowSystem.h"
#include "VulkanTypes.h"
#include "VulkanMacro.h"
#include "VulkanIncludes.h"

namespace Nova {

constexpr VkExtent2D DEFAULT_WINDOW_SIZE { 1280, 720 };
constexpr const char* DEFAULT_WINDOW_TITLE = "LearnVulkan";
constexpr const uint32_t MAX_FRAMES_IN_FLIGHT { 3 };

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
    VkSwapchainKHR           m_swapchain {};
    std::vector<VkImage>     m_swapchain_images {};
    std::vector<VkImageView> m_swapchain_imageviews {};
    VkFormat                 m_swapchain_image_format {};
    VkExtent2D               m_swapchain_extent {};

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

    void Clear() const;

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

    // swapchain
    SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physical_device) const;
    VkSurfaceFormatKHR      ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surface_formats);
    VkPresentModeKHR        ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& present_modes);
    VkExtent2D              ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};
} // namespace Nova