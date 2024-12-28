#include "WindowSystem.h"
#include "Macro.h"

namespace Nova {
WindowSystem::~WindowSystem() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void WindowSystem::Init(const WindowCreateInfo& create_info)
{
    if (!glfwInit()) {
        LOG_FATAL(__FUNCTION__, "Failed to init GLFW");
    }

    m_width = create_info.width;
    m_height = create_info.height;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
    if (!m_window) {
        LOG_FATAL(__FUNCTION__, "Failed to create glfw window");
        glfwTerminate();
        return;
    }
}

void WindowSystem::PollEvent() const {
    glfwPollEvents();
}

bool WindowSystem::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void WindowSystem::SetTitle(const char* title) {
    glfwSetWindowTitle(m_window, title);
}



} // namespace Nova