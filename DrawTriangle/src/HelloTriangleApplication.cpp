#include "HelloTriangleApplication.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

extern const uint32_t WIDTH;
extern const uint32_t HEIGHT;

void HelloTriangleApplication::initWindow()
{
    glfwInit();

    // 这里是对glfw的一些设置,比如同指glfw不要为OpenGL设置上下文等
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    // 前三个参数是窗口相关的,第四个参数是可选择一个指定的显示器显示窗口
    // 第五个参数是跟OpenGL相关的不用管
}

void HelloTriangleApplication::initVulkan()
{
    PrintExtension();

    createInstance();
}

void HelloTriangleApplication::mainLoop()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void HelloTriangleApplication::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};// 不可选的属性
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // 获取窗口扩展信息
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // 这两个成员决定了要启用的全局验证层
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // 创建实例并判断是否成功创建
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }

}

void HelloTriangleApplication::PrintExtension()
{
    uint32_t extensionCount = 0;
    // 通过该函数可记录扩展的数量和扩展的详细信息
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:" << std::endl;

    for(const auto& extension : extensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
}
