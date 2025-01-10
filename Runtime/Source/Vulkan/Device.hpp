#pragma once

#include "Vulkan.hpp"
#include "Types.hpp"

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

    VkDevice GetHandle() const { return m_device; }

private:
    VkDevice               m_device { nullptr };
    const VkPhysicalDevice m_physical_device;
    const Surface&         m_surface;

    std::vector<VkExtensionProperties> m_available_extensions;

    VkQueue m_queue_graphics { nullptr };
    VkQueue m_queue_present { nullptr };
    VkQueue m_queue_compute { nullptr };

    QueueFamilyIndices m_queue_indices {};

    void CheckDeviceRequiredExtensions(
        VkPhysicalDevice                physical_device,
        const std::vector<const char*>& required_extensions
    ) const;
    QueueFamilyIndices QueryQueueFamiliyIndices(VkPhysicalDevice physical_device) const
};
} // namespace Vulkan
