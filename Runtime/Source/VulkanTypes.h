#pragma once
#include "VulkanMacro.h"
namespace Nova {
struct QueueFamilyIndices {
    union {
        struct {
            uint32_t graphics;
            uint32_t present;
            uint32_t compute;
        };
        uint32_t data[3];
    };

    bool IsComplete() const {
        return graphics != VK_QUEUE_FAMILY_IGNORED && present != VK_QUEUE_FAMILY_IGNORED && compute != VK_QUEUE_FAMILY_IGNORED;
    }
};

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> surface_formats;
    std::vector<VkPresentModeKHR>   presentModes;
};
} // namespace Nova