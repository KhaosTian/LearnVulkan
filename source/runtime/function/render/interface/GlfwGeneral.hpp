#pragma once
#include "VkBase.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib") //链接编译所需的静态库

//窗口的指针，全局变量自动初始化为NULL
inline GLFWwindow* window;
//显示器信息的指针
inline GLFWmonitor* monitor;
//窗口标题
inline auto window_title = "LearnVulkan";


inline bool initialize_window(
    const VkExtent2D size,
    const bool full_screen = false,
    const bool is_resizable = true,
    bool limit_frame_rate = true)
{
    if (!glfwInit())
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to initialize GLFW!\n");
        return false;
    }


    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, is_resizable);

    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* pMode = glfwGetVideoMode(monitor);
    window = full_screen
                 ? glfwCreateWindow(pMode->width, pMode->height, window_title, monitor, nullptr)
                 : glfwCreateWindow(static_cast<int>(size.width), static_cast<int>(size.height), window_title, nullptr,
                                    nullptr);

    if (!window)
    {
        std::cout << std::format("[ InitializeWindow ] ERROR\nFailed to create GLFW window!\n");
        glfwTerminate();
        return false;
    }
    return true;
}

inline void terminate_window()
{
    glfwTerminate();
}

inline void set_window_title_with_fps()
{
    static double current_time = 0.0; //初始化局部静态变量
    static double last_time = glfwGetTime();
    static double time_diff;
    static int frame_count = -1;
    static std::stringstream ss;

    current_time = glfwGetTime(); //更新当前时间和帧数
    frame_count++;
    if ((time_diff = current_time - last_time) >= 1)
    {
        ss.precision(1);
        ss << window_title << "     " << std::fixed << frame_count / time_diff << " FPS";
        glfwSetWindowTitle(window, ss.str().c_str()); //当前时间差超过1s更新标题
        ss.str("");
        last_time = glfwGetTime(); //更新上一帧时间
        frame_count = 0; //清空帧数计数器
    }
}

inline void make_window_full_screen()
{
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

inline void make_window_windowed(const VkOffset2D position, const VkExtent2D size)
{
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(window, monitor, position.x, position.y, size.width, size.height, mode->refreshRate);
}
