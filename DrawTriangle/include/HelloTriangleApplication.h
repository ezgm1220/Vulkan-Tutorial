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

    void createInstance(); // ����ʵ��

    void PrintExtension(); // ��ӡ���ǿ��õ���չ
};
