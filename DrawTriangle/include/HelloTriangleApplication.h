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
    // std::optional �ڸ�ֵ֮ǰ�������κ�ֵ,����ͨ��has_value������

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

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;// ��������Ĭ������

    VkDevice device;

    VkQueue graphicsQueue;// �����洢ͼ�ζ���
    VkQueue presentQueue;// �����洢Presentation����

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;// ��Щͼ�����ɽ�������ʵ�ִ����ģ�һ�������������٣����Ǿͻᱻ�Զ�����
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;// �����洢�������еĻ���ͼ��
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;


    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance(); // ����ʵ��

    void PrintExtension(); // ��ӡ���ǿ��õ���չ

    bool checkValidationLayerSupport();// �����������Ĳ��Ƿ����

    std::vector<const char*> getRequiredExtensions();// ��ȡ�����������չ�б�

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);// debug�Ļص�����

    void setupDebugMessenger();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);// ���VkDebugUtilsMessengerCreateInfoEXT����Ϣ

    void pickPhysicalDevice();// ѡ�������豸

    bool isDeviceSuitable(VkPhysicalDevice device);// ��������豸�Ƿ����

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);// Ѱ��������Ҫ�Ķ��д�

    void createLogicalDevice();// �����߼��豸

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
