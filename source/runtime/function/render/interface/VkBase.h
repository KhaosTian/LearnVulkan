#pragma once
#include "VkStart.h"

namespace Nova
{
class GraphicsBase
{
private:
    GraphicsBase() = default;
    ~GraphicsBase() = default;

public:
    GraphicsBase(GraphicsBase&&) = delete;

    static GraphicsBase& Instance()
    {
        static GraphicsBase singleton;
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

    const std::vector<const char*>& GetInstanceLayerNames() const;

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


    
};

inline const std::vector<const char*>& GraphicsBase::GetInstanceLayerNames() const
{
    return mInstanceLayerNames;
}
}
