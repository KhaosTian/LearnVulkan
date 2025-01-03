#include "Window.hpp"
#include "WindowData.hpp"
#include <stdexcept>

namespace Vulkan {
Window::Window(const WindowData& data): m_data(data) {

    if (!glfwInit()) {
        std::runtime_error("Failed to init GLFW");
    }

    // 根据window data初始化窗口状态
    if (data.hide_title) {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, data.resizable ? GLFW_TRUE : GLFW_FALSE);

    // 显示器
    const GLFWmonitor* monitor = data.is_fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    // 创建window
    m_window = glfwCreateWindow(data.width, data.height, data.title->c_str(), nullptr, nullptr);
    if (!m_window) {
        std::runtime_error("Failed to create glfw window");
        glfwTerminate();
        return;
    }

    // 显示模式
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode) {
        glfwSetWindowPos(m_window, (mode->width - data.width) / 2, (mode->height - data.height) / 2);
    }

    // 回调函数
}

Window::~Window() {}
}
