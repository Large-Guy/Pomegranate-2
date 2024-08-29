#include "core.h"

VkInstance Graphics::instance;

const char* Graphics::getAPI() {
    return "Vulkan";
}

void Graphics::init() {
    //Initialize GLFW

    if (!glfwInit()) {
        const char* description;
        int code = glfwGetError(&description);
        throw std::runtime_error("GLFW failed to initialize: " + std::string(description) + " (" + std::to_string(code) + ")");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    //Create vulkan instance

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pomegranate Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Pomegranate Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t  glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // MoltenVK specific
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); // Required by VK_KHR_portability_subset

    createInfo.enabledExtensionCount = (uint32_t)(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}

void Graphics::terminate() {
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();
}