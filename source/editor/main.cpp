#include <runtime/function/render/interface/GlfwGeneral.hpp>

int main()
{
    if (!InitializeWindow(VkExtent2D{1280, 720}))
    {
        return -1;
    }

    while (!glfwWindowShouldClose(kWindow))
    {
        glfwPollEvents();
        UpdateWindowTitleWithFps();
    }

    TerminateWindow();
    return 0;
}
