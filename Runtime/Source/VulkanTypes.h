#pragma once
#include "VulkanMacro.h"
namespace Nova {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    std::optional<uint32_t> compute;

    bool IsComplte() const {
        return graphics.has_value() && present.has_value() && compute.has_value();
    }
};
} // namespace Nova