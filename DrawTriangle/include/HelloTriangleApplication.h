#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

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
    VkDebugUtilsMessengerEXT debugMessenger;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance(); // 创建实例

    void PrintExtension(); // 打印我们可用的扩展

    bool checkValidationLayerSupport();// 检查所有请求的层是否可用

    std::vector<const char*> getRequiredExtensions();// 获取我们所需的扩展列表

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);// debug的回调函数

    void setupDebugMessenger();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);// 填充VkDebugUtilsMessengerCreateInfoEXT的信息
};
