#include "Device.hpp"
#include "Enumerate.hpp"
#include "Surface.hpp"

namespace Vulkan {
Device::Device(
    VkPhysicalDevice                physical_device,
    const Surface&                  surface,
    const std::vector<const char*>& required_extensions,
    const VkPhysicalDeviceFeatures& device_features
):
    m_physical_device(physical_device),
    m_surface(surface) {

    CheckDeviceRequiredExtensions(physical_device, required_extensions);

    m_queue_indices = QueryQueueFamiliyIndices(physical_device);


}
Device::~Device() {
    if (m_device == nullptr) return;
}

void Device::CheckDeviceRequiredExtensions(VkPhysicalDevice physical_device, const std::vector<const char*>& requireds) const {
    auto availables = GetEnumerateVector(physical_device, Nullptr<const char>(), vkEnumerateDeviceExtensionProperties);

    std::set<std::string> required_set(requireds.begin(), requireds.end());
    for (const auto& available: availables) {
        required_set.erase(available.extensionName);
    }

    if (required_set.empty()) {
        LOG_ERROR("Missing required extensions: {}", availables);
    }
}

QueueFamilyIndices Device::QueryQueueFamiliyIndices(VkPhysicalDevice physical_device) const {
    const auto queue_families = GetEnumerateVector(physical_device, vkGetPhysicalDeviceQueueFamilyProperties);

    // match queue family indices
    QueueFamilyIndices indices = {};
    for (int i = 0; i < queue_families.size(); i++) {
        const auto& queue_family = queue_families[i];

        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphics = i;
        if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) indices.compute = i;

        VkBool32 is_present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_surface.GetHandle(), &is_present_support);
        if (is_present_support) indices.present = i;

        if (indices.IsComplete()) break;
    }

    return indices;
}
} // namespace Vulkan