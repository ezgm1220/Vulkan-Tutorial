#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <iostream>
#include <glm/glm.hpp>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG//C++�е��Դ��궨�壬���ж���debug��release�汾
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        // ��������ʲô��ʽ���ڴ��м����������������
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;// binding�����а󶨵�����
        bindingDescription.stride = sizeof(Vertex);// ��һ���������ݵ���һ���������ݵ��ֽ���
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;// ��ÿ������֮���ƶ�����һ������

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        // ������δ���������
        // ��Ϊ�������������ԣ�λ�ú���ɫ������������Ҫ����Attribute Descriptions
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;//����Vulkanÿ����������������ĸ�binding
        attributeDescriptions[0].location = 0;// Location�������ö�����ɫ���������Locationָ��
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);// ʹ��offsetof��������ƫ����

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

};

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

    VkCommandPool commandPool;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    bool framebufferResized = false;

    uint32_t currentFrame = 0;// ֡����

    void initWindow();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void drawFrame();

    void createSyncObjects();

    void cleanupSwapChain();

    void recreateSwapChain();

    void createVertexBuffer();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, 
                      VkMemoryPropertyFlags properties, 
                      VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
