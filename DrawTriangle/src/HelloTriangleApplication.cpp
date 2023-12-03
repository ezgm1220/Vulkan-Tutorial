#include "HelloTriangleApplication.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

extern const uint32_t WIDTH;
extern const uint32_t HEIGHT;

void HelloTriangleApplication::initWindow()
{
    glfwInit();

    // �����Ƕ�glfw��һЩ����,����ָͬglfw��ҪΪOpenGL���������ĵ�
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    // ǰ���������Ǵ�����ص�,���ĸ������ǿ�ѡ��һ��ָ������ʾ����ʾ����
    // ����������Ǹ�OpenGL��صĲ��ù�
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

    VkInstanceCreateInfo createInfo{};// ����ѡ������
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // ��ȡ������չ��Ϣ
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // ��������Ա������Ҫ���õ�ȫ����֤��
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // ����ʵ�����ж��Ƿ�ɹ�����
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }

}

void HelloTriangleApplication::PrintExtension()
{
    uint32_t extensionCount = 0;
    // ͨ���ú����ɼ�¼��չ����������չ����ϸ��Ϣ
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:" << std::endl;

    for(const auto& extension : extensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
}
