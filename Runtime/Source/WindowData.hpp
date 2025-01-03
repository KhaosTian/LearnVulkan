#pragma once
#include <string>

namespace Vulkan {
struct WindowData {
    int          width;
    int          height;
    std::string* title;
    bool         is_fullscreen;
    bool         resizable;
    bool         hide_title;
};
} // namespace Vulkan