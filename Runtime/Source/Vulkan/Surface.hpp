#pragma once

#include "Vulkan.hpp"

namespace Vulkan {

class Instance;
class Window;

class Surface final {
public:
    Surface(Surface&&) = delete;
    explicit Surface(const Instance& instance);
    ~Surface();

    VkSurfaceKHR    GetHandle() const { return m_surface; }
    const Instance& GetInstance() const { return m_instance; }

private:
    const Instance& m_instance;
    VkSurfaceKHR    m_surface;

};

} // namespace Vulkan
