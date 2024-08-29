#include "core.h"

VkInstance Graphics::instance = VK_NULL_HANDLE;
VkPhysicalDevice Graphics::physicalDevice = VK_NULL_HANDLE;
VkDevice Graphics::logicalDevice = VK_NULL_HANDLE;
Graphics::Queues Graphics::queues;
std::vector<const char*> Graphics::deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool Graphics::QueueFamilies::isComplete() const {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

const char* Graphics::getAPI() {
    return "Vulkan";
}

Graphics::QueueFamilies Graphics::getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    //Get queues
    QueueFamilies indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties > families(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, families.data());

    int i = 0;
    for(const auto& queueFamily : families) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if(presentSupport)
        {
            indices.presentFamily = i;
        }
        if(indices.isComplete())
            break;
        i++;
    }
    return indices;
}

bool Graphics::isDeviceValid(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device,&deviceFeatures);

    QueueFamilies indices = getQueueFamilies(device, surface);
    bool allQueues = indices.isComplete();
    bool extensionsSupported = hasExtensionSupport(device);
    bool swapChainAdequate = false;
    if(extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = getSwapChainSupport(device,surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return allQueues && extensionsSupported && swapChainAdequate;
}

VkSurfaceFormatKHR Graphics::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for(const auto& availableFormat : availableFormats) {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR Graphics::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Graphics::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow* window) {
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window,&width,&height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width,capabilities.minImageExtent.width,capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height,capabilities.minImageExtent.height,capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

Graphics::SwapChainSupportDetails Graphics::getSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,nullptr);
    if(formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if(presentModeCount != 0){
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface,&presentModeCount, details.presentModes.data());
    }
    return details;
}

bool Graphics::hasExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for(const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
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

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    uint32_t  glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // MoltenVK specific
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME); // Required by VK_KHR_portability_subset

    instanceCreateInfo.enabledExtensionCount = (uint32_t)(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance.");
    }

    //Create a surface
    VkSurfaceKHR surface;
    GLFWwindow* window = glfwCreateWindow(1, 1, "surface creation window", nullptr, nullptr);
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
    glfwDestroyWindow(window);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance,&deviceCount, nullptr);
    if(deviceCount == 0)
    {
        throw std::runtime_error("Failed to find GPU with Vulkan support!");
    }

    std::cout << "There are: " << deviceCount << " Vulkan supported devices!" << std::endl;

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for(const auto& device : devices) {
        if(isDeviceValid(device,surface)) {
            physicalDevice = device;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to find GPU!");
    }

    std::cout << "Found device" << std::endl;

    //OPTIONAL TODO: Replace with device scoring system

    QueueFamilies indices = getQueueFamilies(physicalDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queuesCreateInfo{};
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queuesCreateInfo.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queuesCreateInfo.data();
    deviceCreateInfo.queueCreateInfoCount = queuesCreateInfo.size();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.enabledLayerCount = 0; //TODO: Implement validation layers

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &queues.graphics);
    vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(),0,&queues.present);

    std::cout << "Vulkan initialized!" << std::endl;
}

void Graphics::terminate() {
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();
}