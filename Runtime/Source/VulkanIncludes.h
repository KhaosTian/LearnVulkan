#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <span>
#include <memory>
#include <functional>
#include <concepts>
#include <format>
#include <chrono>
#include <numeric>
#include <numbers>


#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

#ifdef _WIN32 // 考虑平台是Windows的情况（请自行解决其他平台上的差异）
    #define VK_USE_PLATFORM_WIN32_KHR // 在包含vulkan.h前定义该宏，会一并包含vulkan_win32.h和windows.h
    #define NOMINMAX // 定义该宏可避免windows.h中的min和max两个宏与标准库中的函数名冲突
    #pragma comment(lib, "vulkan-1.lib")
#endif

#include <vulkan/vulkan.h>