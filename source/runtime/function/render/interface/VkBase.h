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
    VkInstance m_Instance;

    //设计一个同时具有vector和hash的容器
    std::vector<const char*> m_InstanceLayerNames;
    std::vector<const char*> m_InstanceExtensionNames;

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
        return m_Instance;
    }

    const std::vector<const char*>& GetInstanceLayerNames() const
    {
        return m_InstanceLayerNames;
    }

    const std::vector<const char*>& GetInstanceExtensionNames() const
    {
        return m_InstanceExtensionNames;
    }

    void AddInstanceLayerName(const char* layer)
    {
        AddNameToContainer(layer, m_InstanceLayerNames);
    }

    void AddInstanceExtensionName(const char* extension)
    {
        AddNameToContainer(extension, m_InstanceExtensionNames);
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
        m_InstanceLayerNames = layerNames;
    }

    VkResult CheckInstanceExtensionNames(std::span<const char*> extensionNames)
    {
        return VK_SUCCESS;
    }

    void SetInstanceExtensionNames(const std::vector<const char*>& extensionNames)
    {
        m_InstanceExtensionNames = extensionNames;
    }

    //debug messenger
    //===========================================================================
    private:
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkResult CreateDebugMessenger()
    {
        return VK_SUCCESS;
    }

    //window surface
    //===========================================================================
private:
    VkSurfaceKHR m_Surface;


    
};
}
