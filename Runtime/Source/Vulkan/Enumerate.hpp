#pragma once
#include "Vulkan.hpp"
#include <vector>
#include <functional>

namespace Vulkan {
// 基础枚举实现
template<typename TValue>
void GetEnumerateVector(VkResult (*enumerate)(uint32_t*, TValue*), std::vector<TValue>& vector) {
    uint32_t count  = 0;
    VkResult result = enumerate(&count, nullptr);
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate count: %d", result);
        return;
    }

    vector.resize(count);
    result = enumerate(&count, vector.data());
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate data: %d", result);
        return;
    }
}

// 单句柄带返回值的枚举实现
template<typename THandle, typename TValue>
void GetEnumerateVector(
    THandle              handle,
    VkResult             (*enumerate)(THandle, uint32_t*, TValue*),
    std::vector<TValue>& vector
) {
    uint32_t count  = 0;
    VkResult result = enumerate(handle, &count, nullptr);
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate count: %d", result);
        return;
    }

    vector.resize(count);
    result = enumerate(handle, &count, vector.data());
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate data: %d", result);
        return;
    }
}

// 单句柄void返回的枚举实现
template<typename THandle, typename TValue>
void GetEnumerateVector(THandle handle, void (*enumerate)(THandle, uint32_t*, TValue*), std::vector<TValue>& vector) {
    uint32_t count = 0;
    enumerate(handle, &count, nullptr);
    vector.resize(count);
    enumerate(handle, &count, vector.data());
}

// 双句柄枚举实现
template<typename THandle1, typename THandle2, typename TValue>
void GetEnumerateVector(
    THandle1             handle1,
    THandle2             handle2,
    VkResult             (*enumerate)(THandle1, THandle2, uint32_t*, TValue*),
    std::vector<TValue>& vector
) {
    uint32_t count  = 0;
    VkResult result = enumerate(handle1, handle2, &count, nullptr);
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate count: %d", result);
        return;
    }

    vector.resize(count);
    result = enumerate(handle1, handle2, &count, vector.data());
    if (result != VK_SUCCESS) {
        LOG_ERROR("Failed to enumerate data: %d", result);
        return;
    }
}

// 返回值版本的便捷函数
template<typename TValue>
std::vector<TValue> GetEnumerateVector(VkResult (*enumerate)(uint32_t*, TValue*)) {
    std::vector<TValue> result;
    GetEnumerateVector(enumerate, result);
    return result;
}

template<typename THandle, typename TValue>
std::vector<TValue> GetEnumerateVector(THandle handle, VkResult (*enumerate)(THandle, uint32_t*, TValue*)) {
    std::vector<TValue> result;
    GetEnumerateVector(handle, enumerate, result);
    return result;
}

template<typename THandle, typename TValue>
std::vector<TValue> GetEnumerateVector(THandle handle, void (*enumerate)(THandle, uint32_t*, TValue*)) {
    std::vector<TValue> result;
    GetEnumerateVector(handle, enumerate, result);
    return result;
}

template<typename THandle1, typename THandle2, typename TValue>
std::vector<TValue>
GetEnumerateVector(THandle1 handle1, THandle2 handle2, VkResult (*enumerate)(THandle1, THandle2, uint32_t*, TValue*)) {
    std::vector<TValue> result;
    GetEnumerateVector(handle1, handle2, enumerate, result);
    return result;
}

} // namespace Vulkan