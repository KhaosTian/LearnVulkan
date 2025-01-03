#pragma once

#include "VulkanIncludes.h"

namespace Nova {
class VulkanUtils {
public:
    static VkImageView CreateImageView(
        VkDevice           device,
        VkImage            image,
        VkFormat           format,
        VkImageAspectFlags image_aspect_flags,
        VkImageViewType    view_type,
        uint32_t           layout_count,
        uint32_t           mip_levels
    );
};

} // namespace Nova