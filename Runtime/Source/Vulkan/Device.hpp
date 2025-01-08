#pragma once

#include "Vulkan.hpp"
#include "Types.h"
#include <vector>
#include <memory>

namespace Vulkan {

class Surface;

class Device final {
public:
    Device(Device&&) = delete;
    Device(
        VkPhysicalDevice                physical_device,
        const Surface&                  surface,
        const std::vector<const char*>& required_extensions,
        const VkPhysicalDeviceFeatures& device_features
    );
    ~Device();

    VkDevice GetHandle() { return m_device; }

private:
    VkDevice       m_device {nullptr};
    const VkPhysicalDevice m_physical_device;
    const Surface& m_surface;

    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices {};

    void CheckDeviceExtensionsSupport(
        VkPhysicalDevice                physical_device,
        const std::vector<const char*>& requiredExtensions
    ) const;
};
} // namespace Vulkan
