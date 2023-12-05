#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <set>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

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

};
