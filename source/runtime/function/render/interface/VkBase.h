#pragma once
#include "VkStart.h"

namespace Nova
{
class VulkanManager
{
private:
    VulkanManager() = default;
    ~VulkanManager() = default;

public:
    VulkanManager(VulkanManager&&) = delete;

    static VulkanManager& GetSingleton()
    {
        static VulkanManager singleton;
        return singleton;
    }

    //vulkan instance & layers & extensions
    //===========================================================================
private:
    VkInstance mInstance;

    //设计一个同时具有vector和hash的容器
    std::vector<const char*> mInstanceLayerNames;
    std::vector<const char*> mInstanceExtensionNames;

    static void AddNameToContainer(const char* name, std::vector<const char*>& container)
    {
        for (const auto& item : container)
        {
            if (!strcmp(name, item))
            {
                return;
            }
        }
        container.push_back(name);
    }

public:
    VkInstance GetInstance() const
    {
        return mInstance;
    }

    const std::vector<const char*>& GetInstanceLayerNames() const
    {
        return mInstanceLayerNames;
    }

    const std::vector<const char*>& GetInstanceExtensionNames() const
    {
        return mInstanceExtensionNames;
    }

    void AddInstanceLayerName(const char* layer)
    {
        AddNameToContainer(layer, mInstanceLayerNames);
    }

    void AddInstanceExtensionName(const char* extension)
    {
        AddNameToContainer(extension, mInstanceExtensionNames);
    }

    VkResult CreateInstance(VkInstanceCreateFlags flags = 0)
    {
#ifndef NDEBUG
        AddInstanceLayerName("VK_LAYER_KHRONOS_validation");
        AddInstanceExtensionName(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
        
        return VK_SUCCESS;
    }

    VkResult CheckInstanceLayerNames(std::span<const char*> layerNames)
    {
        return VK_SUCCESS;
    }

    void SetInstanceLayerNames(const std::vector<const char*>& layerNames)
    {
        mInstanceLayerNames = layerNames;
    }

    VkResult CheckInstanceExtensionNames(std::span<const char*> extensionNames)
    {
        return VK_SUCCESS;
    }

    void SetInstanceExtensionNames(const std::vector<const char*>& extensionNames)
    {
        mInstanceExtensionNames = extensionNames;
    }

    //debug messenger
    //===========================================================================
private:
    VkDebugUtilsMessengerEXT mDebugMessenger;

    VkResult CreateDebugMessenger()
    {
        return VK_SUCCESS;
    }

    //window surface
    //===========================================================================
private:
    VkSurfaceKHR mSurface;

public:
    VkSurfaceKHR GetSurface() const
    {
        return mSurface;
    }

    void Surface(VkSurfaceKHR surface)
    {
        if (!mSurface)
        {
            mSurface = surface;
        }
    }

    //devices
    //===========================================================================
private:
    VkPhysicalDevice mPhysicalDevice;
    VkPhysicalDeviceProperties mPhysicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
    std::vector<VkPhysicalDevice> mAvailablePhysicalDevices;

    VkDevice mDevice;
    uint32_t mQueueFamilyIndexGraphics = VK_QUEUE_FAMILY_IGNORED;
    uint32_t mQueueFamilyIndexPresentation = VK_QUEUE_FAMILY_IGNORED;
    uint32_t mQueueFamilyIndexCompute = VK_QUEUE_FAMILY_IGNORED;

    std::vector<const char*> mDeviceExtensionNames;

public:
    VkPhysicalDevice GetPhysicalDevice() const
    {
        return mPhysicalDevice;
    }

    const VkPhysicalDeviceProperties& GetPhysicalDeviceProperties() const
    {
        return mPhysicalDeviceProperties;
    }

    const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const
    {
        return mPhysicalDeviceMemoryProperties;
    }

    VkDevice GetDevice() const
    {
        return mDevice;
    }

    VkPhysicalDevice GetAvailablePhysicalDevice(uint32_t index) const
    {
        return mAvailablePhysicalDevices[index];
    }

    uint32_t GetAvailablePhysicalDeviceCount() const
    {
        return static_cast<uint32_t>(mAvailablePhysicalDevices.size());
    }

    uint32_t GetQueueFamilyIndexGraphics() const
    {
        return mQueueFamilyIndexGraphics;
    }

    uint32_t GetQueueFamilyIndexPresentation() const
    {
        return mQueueFamilyIndexPresentation;
    }

    uint32_t GetQueueFamilyIndexCompute() const
    {
        return mQueueFamilyIndexCompute;
    }

    const std::vector<const char*>& GetDeviceExtensions() const
    {
        return mDeviceExtensionNames;
    }

    void AddDeviceExtension(const char* extensionName)
    {
        AddNameToContainer(extensionName, mDeviceExtensionNames);
    }

    VkResult GetPhysicalDevice()
    {
        return VK_SUCCESS;
    }

    VkResult DeterminePhysicalDevice(uint32_t deviceIndex = 0, bool enableGraphicsQueue = true,
                                     bool enableCompute = true)
    {
        return VK_SUCCESS;
    }

    VkResult CreateDevice()
    {
        return VK_SUCCESS;
    }

    VkResult CheckDeviceExtensionNames(std::span<const char*> extensionNamesToCheck,
                                       const char* layerName = nullptr) const
    {
        return VK_SUCCESS;
    }

    void SetDeviceExtensionNames(const std::vector<const char*>& extensionNames)
    {
        mDeviceExtensionNames = extensionNames;
    }

    //swap chain
    //===========================================================================
private:
    std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;

    VkSwapchainKHR mSwapChain;
    std::vector<VkImage> mSwapChainImages;
    std::vector<VkImageView> mSwapChainImageViews;

    VkSwapchainCreateInfoKHR mSwapChainCreateInfo = {};

    VkResult CreateSwapchainInternal()
    {
        return VK_SUCCESS;
    }

public:
    const VkFormat& GetAvailableSurfaceFormat(uint32_t index) const
    {
        return mAvailableSurfaceFormats[index].format;
    }

    const VkColorSpaceKHR& GetAvailableSurfaceColorSpace(uint32_t index) const
    {
        return mAvailableSurfaceFormats[index].colorSpace;
    }

    uint32_t GetAvailableSurfaceFormatCount() const
    {
        return static_cast<uint32_t>(mAvailableSurfaceFormats.size());
    }

    VkSwapchainKHR GetSwapChain() const
    {
        return mSwapChain;
    }

    VkImage GetSwapChainImage(uint32_t index) const
    {
        return mSwapChainImages[index];
    }

    uint32_t GetSwapChainImageCount() const
    {
        return static_cast<uint32_t>(mSwapChainImages.size());
    }

    VkImageView GetSwapChainImageView(uint32_t index) const
    {
        return mSwapChainImageViews[index];
    }

    const VkSwapchainCreateInfoKHR& GetSwapChainCreateInfo() const
    {
        return mSwapChainCreateInfo;
    }

    VkResult GetSurfaceFormats()
    {
        return VK_SUCCESS;
    }

    VkResult CreateSwapchain(bool limitFrameRate = true, VkSwapchainCreateFlagsKHR flags = 0)
    {
        return VK_SUCCESS;
    }

    VkResult RecreateSwapChain()
    {
        return VK_SUCCESS;
    }

    //vulkan version
    //===========================================================================
private:
    uint32_t mApiVersion = VK_API_VERSION_1_0;

public:
    uint32_t GetApiVersion() const
    {
        return mApiVersion;
    }

    VkResult UseLatestApiVersion()
    {
        return VK_SUCCESS;
    }


};
};
