#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "WindowSystem.h"

namespace Nova {

struct RenderCoreInitInfo {
    std::shared_ptr<WindowSystem> window_system;
};

class RenderCore {
private:
    GLFWwindow* m_window { nullptr };

    VkInstance m_instance { nullptr };

    std::vector<const char*> mInstanceLayers;
    std::vector<const char*> mInstanceExtensions;

    bool m_enable_validation_layers { true };

public:
    void Init(RenderCoreInitInfo init_info);
    void CreateInstance();
    void InitDebugMessenger();
    void CreateWindowSurface();
    void InitPhysicalDevice();
    void CreateLogicalDevice();
};
} // namespace Nova