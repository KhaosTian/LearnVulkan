#include "SwapChain.hpp"
#include "Device.hpp"
#include "Enumerate.hpp"

namespace Vulkan {

SwapChain::SwapChain(const Device& device, const VkPresentModeKHR present_mode)
    : m_device(device), m_physical_device(device.GetPhysicalDevice()), m_surface(device.GetSurface())
{
    
}

SwapChain::~SwapChain() {
}

SwapchainSupportDetails SwapChain::GetSupportDetails() const {
    SwapchainSupportDetails details = {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, m_surface, &details.capabilities);
    details.formats = GetEnumerateVector(m_physical_device, m_surface, vkGetPhysicalDeviceSurfaceFormatsKHR);
    details.present_modes = GetEnumerateVector(m_physical_device, m_surface, vkGetPhysicalDeviceSurfacePresentModesKHR);
    return details;
}
}