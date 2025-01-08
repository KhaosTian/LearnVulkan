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

}
Device::~Device() {
}
void Device::CheckDeviceExtensionsSupport(
    VkPhysicalDevice                physical_device,
    const std::vector<const char*>& requiredExtensions
) const {
    const auto available_extensions = GetEnumerateVector(physical_device, static_cast<const char*>(nullptr), vkEnumerateDeviceExtensionProperties);
}
} // namespace Vulkan