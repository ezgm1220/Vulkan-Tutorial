#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class HelloTriangleApplication
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;

    VkInstance instance;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance(); // 创建实例

    void PrintExtension(); // 打印我们可用的扩展
};
