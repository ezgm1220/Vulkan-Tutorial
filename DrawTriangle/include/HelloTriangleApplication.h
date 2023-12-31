#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES// 这将迫使GLM使用已经为我们指定了对齐要求的vec2和mat4的版本
#define GLM_FORCE_DEPTH_ZERO_TO_ONE// 使用Vulkan的深度范围为0.0到1.0
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <optional>
#include <set>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG//C++中的自带宏定义，来判断是debug和release版本
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        // 描述了以什么形式从内存中加载整个顶点的数据
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;// binding数组中绑定的索引
        bindingDescription.stride = sizeof(Vertex);// 从一个顶点数据到下一个顶点数据的字节数
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;// 在每个顶点之后移动到下一个顶点

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        // 描述如何处理顶点输入
        // 因为我们有两个属性，位置和颜色，所以我们需要两个Attribute Descriptions
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;//告诉Vulkan每个顶点的数据来自哪个binding
        attributeDescriptions[0].location = 0;// Location参数引用顶点着色器中输入的Location指令
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);// 使用offsetof宏来计算偏移量

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const
    {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

};

namespace std
{
    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    // 纹理相关
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    // 深度测试相关
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    bool framebufferResized = false;

    uint32_t currentFrame = 0;// 帧索引

    void initWindow();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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

    // 将command Buffer 的流程拆分开来
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createIndexBuffer();

    void createDescriptorSetLayout();

    void createUniformBuffers();

    void updateUniformBuffer(uint32_t currentImage);

    void createDescriptorPool();

    void createDescriptorSets();

    void createTextureImage();

    void createImage(uint32_t width, uint32_t height, 
                     VkFormat format, VkImageTiling tiling, 
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
                     VkImage& image, VkDeviceMemory& imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureImageView();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void createTextureSampler();// 创建纹理采样器

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, 
                                 VkImageTiling tiling, VkFormatFeatureFlags features);
    
    // 为深度测试寻找最合适的格式
    VkFormat findDepthFormat();

    // 判断是否包含模板组件
    bool hasStencilComponent(VkFormat format);

    void loadModel();

};
