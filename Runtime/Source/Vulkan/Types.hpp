#pragma once

#include "Vulkan.hpp"

namespace Vulkan {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    std::optional<uint32_t> compute;

    bool IsComplete() const { return graphics.has_value() && present.has_value() && compute.has_value(); }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   present_modes;
};
}
