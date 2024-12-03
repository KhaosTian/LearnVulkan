#pragma once
#include "VkBase.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/// GLFW窗口指针，全局变量，默认为NULL
inline GLFWwindow* kWindow;

/// 显示器信息指针
inline GLFWmonitor* kMonitor;

/// 窗口标题
inline auto kWindowTitle = "LearnVulkan";

/**
 * 初始化GLFW并创建窗口。
 *
 * @param size 窗口的大小
 * @param fullScreen 是否全屏，默认为false
 * @param isResizable 是否可调整窗口大小，默认为true
 * @param enableFpsLimit 是否限制帧率，默认为true
 * @return 初始化是否成功
 */
inline bool InitializeWindow(
    const VkExtent2D size,
    const bool fullScreen = false,
    const bool isResizable = true,
    bool enableFpsLimit = true)
{
    auto& vm = Nova::VulkanManager::GetSingleton();
    
    if (!glfwInit())
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to initialize GLFW!\n");
        return false;
    }

    // 设置窗口属性
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizable);

    uint32_t extensionCount = 0;
    const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
    if(!extensionNames)
    {
        std::cout<<std::format("[ InitializeWindow ] ERROR\nFailed to get GLFW required extensions!\n");
        glfwTerminate();
        return false;
    }

    for(size_t i=0; i < extensionCount; i++)
    {
        vm.AddInstanceExtensionName(extensionNames[i]);
    }

    //window surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if(VkResult result = glfwCreateWindowSurface(vm.GetInstance(), kWindow, nullptr, &surface))
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to create GLFW surface!\n");
        glfwTerminate();
        return false;
    }

    vm.SetSurface(surface);

    
    kMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* pMode = glfwGetVideoMode(kMonitor);

    // 创建窗口
    kWindow = fullScreen
        ? glfwCreateWindow(pMode->width, pMode->height, kWindowTitle, kMonitor, nullptr)
        : glfwCreateWindow(static_cast<int>(size.width), static_cast<int>(size.height), kWindowTitle, nullptr, nullptr);

    if (!kWindow)
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to create GLFW window!\n");
        glfwTerminate();
        return false;
    }
    return true;
}

/**
 * 终止GLFW并清理资源。
 */
inline void TerminateWindow()
{
    glfwTerminate();
}

/**
 * 设置窗口标题并显示当前FPS。
 */
inline void UpdateWindowTitleWithFps()
{
    static double currentTime = 0.0; // 当前时间
    static double lastTime = glfwGetTime();
    static double timeDiff;
    static int frameCount = -1;
    static std::stringstream ss;

    currentTime = glfwGetTime(); // 获取当前时间
    frameCount++;

    if ((timeDiff = currentTime - lastTime) >= 0.1)
    {
        ss.precision(1);
        ss << kWindowTitle << "     " << std::fixed << frameCount / timeDiff << " FPS";
        glfwSetWindowTitle(kWindow, ss.str().c_str()); // 更新窗口标题
        ss.str("");
        lastTime = currentTime; // 更新上一帧时间
        frameCount = 0; // 重置帧计数器
    }
}

/**
 * 将窗口设置为全屏模式。
 */
inline void MakeWindowFullScreen()
{
    const GLFWvidmode* mode = glfwGetVideoMode(kMonitor);
    glfwSetWindowMonitor(kWindow, kMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

/**
 * 指定窗口位置和大小，并更新显示模式
 *
 * @param position 窗口位置
 * @param size 窗口大小
 */
inline void RestoreWindow(const VkOffset2D position, const VkExtent2D size)
{
    const GLFWvidmode* mode = glfwGetVideoMode(kMonitor);
    glfwSetWindowMonitor(kWindow, kMonitor,
                         position.x, position.y,
                         static_cast<int>(size.width), static_cast<int>(size.height),
                         mode->refreshRate);
}