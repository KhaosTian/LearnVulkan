#pragma once
#include "VulkanIncludes.h"

namespace Nova {
VKAPI_ATTR VkResult VKAPI_CALL _vkCreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks*              pAllocator,
    VkDebugUtilsMessengerEXT*                 pDebugMessenger
) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
    );
    if (func) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void _vkDestroyDebugUtilsMessengerEXT(
    VkInstance                   instance,
    VkDebugUtilsMessengerEXT     debug_messenger,
    const VkAllocationCallbacks* pAllocator
) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
    );
    if (func) {
        func(instance, debug_messenger, pAllocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL _vkDebugUtilsMessengerCallsbackEXT(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*                                       pUserData
) {
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << "\n";
    return VK_FALSE;
};

VKAPI_ATTR VkResult VKAPI_CALL
_vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
    auto func = reinterpret_cast<PFN_vkWaitForFences>(vkGetDeviceProcAddr(device, "vkWaitForFences"));
    if (func) {
        return func(device, fenceCount, pFences, waitAll, timeout);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

} // namespace Nova