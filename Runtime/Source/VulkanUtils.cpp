#include "VulkanUtils.h"

VkImageView Nova::VulkanUtils::CreateImageView(
    VkDevice           device,
    VkImage            image,
    VkFormat           format,
    VkImageAspectFlags image_aspect_flags,
    VkImageViewType    view_type,
    uint32_t           layout_count,
    uint32_t           mip_levels
) {
    VkImageViewCreateInfo create_info           = {};
    create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image                           = image;
    create_info.viewType                        = view_type;
    create_info.format                          = format;
    create_info.subresourceRange.aspectMask     = image_aspect_flags;
    create_info.subresourceRange.baseMipLevel   = 0;
    create_info.subresourceRange.levelCount     = mip_levels;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount     = layout_count;

    VkImageView imageview;
    vkCreateImageView(device, &create_info, nullptr, &imageview);
    return imageview;
}