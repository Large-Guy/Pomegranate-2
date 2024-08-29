#include "core.h"

VkInstance Graphics::instance = VK_NULL_HANDLE;
VkPhysicalDevice Graphics::physicalDevice = VK_NULL_HANDLE;
VkDevice Graphics::logicalDevice = VK_NULL_HANDLE;
VkQueue Graphics::graphicsQueue = VK_NULL_HANDLE;

bool Graphics::QueueFamilies::isComplete() const {
    return graphicsFamily.has_value();
}

const char* Graphics::getAPI() {
    return "Vulkan";
}

Graphics::QueueFamilies Graphics::getQueueFamilies(VkPhysicalDevice device) {
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
        if(indices.isComplete())
            break;
        i++;
    }
    return indices;
}

bool Graphics::isDeviceValid(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device,&deviceFeatures);

    QueueFamilies indices = getQueueFamilies(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && indices.isComplete();
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
        if(isDeviceValid(device)) {
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

    QueueFamilies indices = getQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.enabledLayerCount = 0; //TODO: Implement validation layers

    if(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);

    std::cout << "Vulkan initialized!" << std::endl;
}

void Graphics::terminate() {
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();
}