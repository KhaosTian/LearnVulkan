#pragma once

#include "Vulkan.hpp"
#include "Types.hpp"

namespace Vulkan {
class Device;
class ImageView;
class Window;

class SwapChain final {
public:
    SwapChain(const Device& device, const VkPresentModeKHR present_mode);
    ~SwapChain();
    SwapChain(SwapChain&&) = delete;

    VkSwapchainKHR GetHandle() const{ return m_swapchain;}
private:
    SwapchainSupportDetails GetSupportDetails() const;

    
    VkSwapchainKHR m_swapchain{nullptr};
    const Device& m_device;
    const VkPhysicalDevice m_physical_device{nullptr};
    const VkSurfaceKHR m_surface{nullptr};
};
}