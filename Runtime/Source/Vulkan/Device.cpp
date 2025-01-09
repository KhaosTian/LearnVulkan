#include "Device.hpp"
#include "Enumerate.hpp"

namespace Vulkan {
Device::Device(
    VkPhysicalDevice                physical_device,
    const Surface&                  surface,
    const std::vector<const char*>& required_extensions,
    const VkPhysicalDeviceFeatures& device_features
):
    m_physical_device(physical_device),
    m_surface(surface) {
    CheckDeviceExtensions(required_extensions);
}
Device::~Device() {
    if (m_device == nullptr) return;
}

void Device::CheckDeviceExtensions(const std::vector<const char*>& required_extensions) const {
    auto availables =
        GetEnumerateVector(m_physical_device, Nullptr<const char>(), vkEnumerateDeviceExtensionProperties);

    std::set<std::string> requireds(required_extensions.begin(), required_extensions.end());
    for (const auto& available: availables) {
        requireds.erase(available.extensionName);
    }

    if (requireds.empty()) {
        LOG_ERROR("Missing required extensions: {}", availables);
    }
}
} // namespace Vulkan