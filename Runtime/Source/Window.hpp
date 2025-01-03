#pragma once

#include "Vulkan.hpp"

#include <functional>
#include <vector>

namespace Vulkan {

class WindowData;

class Window final {
public:
    Window(Window&&) = delete;
    explicit Window(const WindowData& data);
    ~Window();

    GLFWwindow*       Handle() const { return m_window; }
    const WindowData& GetData() const { return m_data; }

    VkExtent2D               WindowSize() const;
    std::vector<const char*> GetRequiredInstanceExtensions() const;

private:
    const WindowData m_data;
    GLFWwindow*      m_window {};
};

} // namespace Vulkan
