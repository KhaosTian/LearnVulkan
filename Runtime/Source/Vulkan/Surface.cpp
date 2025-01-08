#include "Surface.hpp"
#include "Instance.hpp"
#include "Window.hpp"

namespace Vulkan {
Surface::Surface(const Instance& instance): m_instance(instance) {
    Check(
        glfwCreateWindowSurface(instance.GetHandle(), instance.GetWindow().Handle(), nullptr, &m_surface),
        "create window surface"
    );
}

Surface::~Surface() {
    if (m_surface == nullptr) {
        return;
    }

    vkDestroySurfaceKHR(m_instance.GetHandle(), m_surface, nullptr);
    m_surface = nullptr;
}
} // namespace Vulkan
