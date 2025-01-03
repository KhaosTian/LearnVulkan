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

private:
    const WindowData m_data;
    GLFWwindow*      m_window {};
};

} // namespace Vulkan
