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
    std::vector<const char*> mInstanceLayers;
    std::vector<const char*> mInstanceExtensions;

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
        return mInstanceLayers;
    }

    const std::vector<const char*>& GetInstanceExtensionNames() const
    {
        return mInstanceExtensions;
    }

    void AddInstanceLayerName(const char* layer)
    {
        AddNameToContainer(layer, mInstanceLayers);
    }

    void AddInstanceExtensionName(const char* extension)
    {
        AddNameToContainer(extension, mInstanceExtensions);
    }

    VkResult CreateInstance(VkInstanceCreateFlags flags = 0)
    {
#ifndef NDEBUG
        AddInstanceLayerName("VK_LAYER_KHRONOS_validation");
        AddInstanceExtensionName(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        VkApplicationInfo applicationInfo =
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .apiVersion = mApiVersion,
        };

        VkInstanceCreateInfo instanceCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = flags,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<uint32_t>(mInstanceLayers.size()),
            .ppEnabledLayerNames = mInstanceLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(mInstanceExtensions.size()),
            .ppEnabledExtensionNames = mInstanceExtensions.data(),
        };

        if (const VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance))
        {
            std::cout << std::format("[ VulkanManager ] Failed to create instance: ") << result << '\n';
            return result;
        }

        std::cout << std::format(
            "Vulkan API Version: {}.{}.{}\n",
            VK_VERSION_MAJOR(mApiVersion),
            VK_VERSION_MINOR(mApiVersion),
            VK_VERSION_PATCH(mApiVersion));

#ifndef NDEBUG
        CreateDebugMessenger();
#endif
        return VK_SUCCESS;
    }

    VkResult CheckInstanceLayers(std::span<const char*> layersToCheck)
    {
        uint32_t layerCount;
        if (VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr))
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate instance layer properties: ") << result <<
                '\n';
            return result;
        }

        if (layerCount == 0)
        {
            for (auto& layerName : layersToCheck)
            {
                layerName = nullptr;
            }
            return VK_SUCCESS;
        }

        std::vector<VkLayerProperties> availableLayers;
        availableLayers.resize(layerCount);
        if (VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()))
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate instance layer properties: ") << result <<
                '\n';
            return result;
        }

        for (auto& layerName : layersToCheck)
        {
            bool found = false;
            for (auto& availableLayer : availableLayers)
            {
                if (!strcmp(layerName, availableLayer.layerName))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                layerName = nullptr;
            }
        }

        return VK_SUCCESS;
    }

    void SetInstanceLayerNames(const std::vector<const char*>& layerNames)
    {
        mInstanceLayers = layerNames;
    }

    VkResult CheckInstanceExtensionNames(std::span<const char*> extensionNames)
    {
        uint32_t extensionCount;
        if (VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr))
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate instance extension properties: ") << result
                << '\n';
            return result;
        }
        if (extensionCount == 0)
        {
            for (auto& extensionName : extensionNames)
            {
                extensionName = nullptr;
            }
            return VK_SUCCESS;
        }
        std::vector<VkExtensionProperties> availableExtensions;
        availableExtensions.resize(extensionCount);
        if (VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                                                     availableExtensions.data()))
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate instance extension properties: ") << result
                << '\n';
            return result;
        }
        for (auto& extensionName : extensionNames)
        {
            bool found = false;
            for (auto& availableExtension : availableExtensions)
            {
                if (!strcmp(extensionName, availableExtension.extensionName))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                extensionName = nullptr;
            }
        }
        return VK_SUCCESS;
    }

    void SetInstanceExtensionNames(const std::vector<const char*>& extensionNames)
    {
        mInstanceExtensions = extensionNames;
    }

    //debug messenger
    //===========================================================================
private:
    VkDebugUtilsMessengerEXT mDebugMessenger;

    VkResult CreateDebugMessenger()
    {
        static PFN_vkDebugUtilsMessengerCallbackEXT DebugUtilsMessengerCallback = [](
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) -> VkBool32
        {
            std::cout << std::format("[ VulkanManager ] Debug: ") << pCallbackData->pMessage << '\n';
            return VK_FALSE;
        };

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = DebugUtilsMessengerCallback,
        };

        const auto vkCreateDebugUtilsMessengerExt = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
        if (vkCreateDebugUtilsMessengerExt)
        {
            const VkResult result = vkCreateDebugUtilsMessengerExt(mInstance, &debugUtilsMessengerCreateInfo, nullptr,
                                                                   &mDebugMessenger);
            if (result)
            {
                std::cout << std::format("[ VulkanManager ] Failed to create debug messenger: ") << result << '\n';
            }
            return result;
        }

        std::cout << std::format("[ VulkanManager ] Failed to get vkCreateDebugUtilsMessengerEXT\n");

        //没有合适的错误代码时就返回 VK_RESULT_MAX_ENUM
        return VK_RESULT_MAX_ENUM;
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

    void SetSurface(VkSurfaceKHR surface)
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
    VkResult GetPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        VkResult result = vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr)
        if (result != VK_SUCCESS)
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate physical devices: ") << result << '\n';
            return result;
        }
        if (deviceCount == 0)
        {
            std::cout << std::format("[ VulkanManager ] Failed to find any physical devices\n");
            abort();
        }
        
        mAvailablePhysicalDevices.resize(deviceCount);
        result = vkEnumeratePhysicalDevices(mInstance, &deviceCount, mAvailablePhysicalDevices.data());
        if (result != VK_SUCCESS)
        {
            std::cout << std::format("[ VulkanManager ] Failed to enumerate physical devices: ") << result << '\n';
        }
        return result;
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
        if (vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion"))
        {
            return vkEnumerateInstanceVersion(&mApiVersion);
        }
        return VK_SUCCESS;
    }
};
};
