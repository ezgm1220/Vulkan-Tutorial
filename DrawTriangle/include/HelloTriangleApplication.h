#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <set>
#include <iostream>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    // std::optional 在赋值之前不包含任何值,可以通过has_value来区分

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

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
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;// 这个对象会默认销毁

    VkDevice device;

    VkQueue graphicsQueue;// 用来存储图形队列
    VkQueue presentQueue;// 用来存储Presentation队列

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;// 这些图像是由交换链的实现创建的，一旦交换链被销毁，它们就会被自动清理
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;// 用来存储交换链中的基本图像
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;


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

    void pickPhysicalDevice();// 选择物理设备

    bool isDeviceSuitable(VkPhysicalDevice device);// 检查物理设备是否合适

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);// 寻找我们需要的队列簇

    void createLogicalDevice();// 创建逻辑设备

    void createSurface();

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR  chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR    chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D          chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createSwapChain();

    void createImageViews();

    void createGraphicsPipeline();

    static std::vector<char>readFile(const std::string& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createRenderPass();

    void createFramebuffers();
};
