#pragma once

// std
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <string>

// external
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#define VK_USE_PLATFORM_WIN32_KHR
#pragma comment(lib, "vulkan-1.lib")
#include <vulkan/vulkan.h>