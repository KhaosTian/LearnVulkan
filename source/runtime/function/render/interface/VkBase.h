#pragma once
#include "VkStart.h"

namespace Nova {
class GraphicsBase {
private:
    GraphicsBase() = default;
    ~GraphicsBase() = default;

public:
    GraphicsBase(GraphicsBase&&) = delete;

    static GraphicsBase& Instance() {
        static GraphicsBase singleton;
        return singleton;
    }

    VkInstance m_VkInstance;

    //设计一个同时具有vector和hash的容器
    std::vector<const char*> m_InstanceLayerNames;
    std::vector<const char*> m_InstanceExtensionNames;

    static void AddNameToContainer(const char* name, std::vector<const char*>& container) {
        for (const auto& item: container) {
            if (!strcmp(name, item)) {
                return;
            }
        }
        container.push_back(name);
    }


    VkInstance GetVkInstance() const {
        return m_VkInstance;
    }

    const std::vector<const char*>& GetVkInstanceLayerNames() const {
        return m_InstanceLayerNames;
    }

    const std::vector<const char*>& GetVkInstanceExtensionNames() const {
        return m_InstanceExtensionNames;
    }

    void AddInstanceLayerName(const char* layer) {
        AddNameToContainer(layer, m_InstanceLayerNames);
    }

    void Add_instanceExtensionName(const char* extension) {
        AddNameToContainer(extension, m_InstanceExtensionNames);
    }

    VkResult CreateVkInstance(VkInstanceCreateFlags flags = 0) {
        
    }

    VkResult CheckVkInstanceLayerNames(std::span<const char*> layers) {
        
    }

    void set_instance_layers(std::vector<const char*> layers_to_set) {
        m_InstanceLayerNames = layers_to_set;
    }

    
};
}