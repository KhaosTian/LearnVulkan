#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <functional>
#include <vector>

namespace Nova {

struct WindowCreateInfo {
    int         width { 1280 };
    int         height { 720 };
    const char* title { "Nova" };
    bool        is_fullscreen { false };
};

class WindowSystem {
private:
    GLFWwindow* m_window { nullptr };

    int m_width { 0 };
    int m_height { 0 };

public:
    WindowSystem() = default;
    ~WindowSystem();

    void Init(const WindowCreateInfo&);
    void PollEvent() const;
    bool ShouldClose() const;
    void SetTitle(const char* title);
    
    GLFWwindow* GetWindow() const {
        return m_window;
    }

    int GetWidth() const {
        return m_width;
    }
    int GetHeight() const {
        return m_height;
    }
};

} // namespace Nova