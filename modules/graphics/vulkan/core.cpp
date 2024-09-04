#include "core.h"

Graphics* Graphics::_graphicsInstance = nullptr;
bool Graphics::enableValidationLayers = false;

Graphics::Graphics() {
    _instance = VK_NULL_HANDLE;
    _physicalDevice = VK_NULL_HANDLE;
    _logicalDevice = VK_NULL_HANDLE;
    _queues = {};
    validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    Debug::AssertIf::isFalse(glfwInit(), "Failed to initialize GLFW");
    Graphics::createInstance(enableValidationLayers);
}

Graphics::~Graphics() {
    vkDestroyDevice(_logicalDevice, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwTerminate();
}

void Graphics::createInstance(bool enableValidationLayers) {

    if(enableValidationLayers)
    {
        Debug::WarnIf::isFalse(hasValidationLayerSupport(), "Validation layers requested, but not available! Continuing without validation layers.");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pomegranate Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Pomegranate Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //Extensions

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    //GLFW Extensions

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.push_back(glfwExtensions[i]);
    }

    //Portability Extensions
    requiredExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    //Ensure extensions are supported

    for (const char* extension : requiredExtensions) {
        bool found = false;

        for (const auto& availableExtension : extensions) {
            if (strcmp(availableExtension.extensionName, extension) == 0) {
                found = true;
                break;
            }
        }

        Debug::AssertIf::isFalse(found, String("Required extension not supported: ") + extension);
    }

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    Debug::AssertIf::isFalse(vkCreateInstance(&createInfo, nullptr, &_instance) == VK_SUCCESS, "Failed to create Vulkan instance");
}

void Graphics::createPhysicalDevice() {
    if(_physicalDevice != VK_NULL_HANDLE)
        return;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance,&deviceCount, nullptr);

    Debug::AssertIf::isZero(deviceCount,"Unable to find GPU(s) with Vulkan support!");
    Debug::Log::pass(String("Found ") + String((int)deviceCount) + " devices with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

    for(const auto& device : devices)
    {
        if(isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
        }
    }

    Debug::AssertIf::isNull(_physicalDevice, "Failed to find suitable GPU!");
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(_physicalDevice, &deviceProperties);
    Debug::Log::pass(String("Found suitable GPU - ") + deviceProperties.deviceName);
}

void Graphics::createLogicalDevice(bool enableValidationLayers) {
    if(_logicalDevice != VK_NULL_HANDLE)
        return;

    QueueFamilyIndices indices = getQueueFamilies(_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    //Portability Subset extension
    const char* extensions[] = {VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME};

    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;


    createInfo.pEnabledFeatures = &deviceFeatures;

    if(enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    Debug::AssertIf::isFalse(vkCreateDevice(_physicalDevice,&createInfo, nullptr,&_logicalDevice) == VK_SUCCESS, "Failed to create logical device");
    Debug::Log::pass("Created logical device.");

    vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0, &_queues.graphicsQueue);
    vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0, &_queues.presentQueue);
    Debug::Log::pass("Created queues.");
}

bool Graphics::hasValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

    for(const char* layerName : validationLayers) {
        bool layerFound = false;

        for(const auto& layerProperties : availableLayers) {
            if(strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        if(!layerFound)
        {
            return false;
        }
    }

    return true;
}

bool Graphics::hasDeviceExtensionSupport(VkPhysicalDevice device) {

}

bool Graphics::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = getQueueFamilies(device);

    bool extensionSupported = hasDeviceExtensionSupport(device);

    return indices.complete() && extensionSupported;
}

Graphics::QueueFamilyIndices Graphics::getQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties > queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for(const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _currentSurface, &presentSupport);

        if(presentSupport)
        {
            indices.presentFamily = i;
        }

        if(indices.complete())
            break;

        i++;
    }

    return indices;
}

bool Graphics::QueueFamilyIndices::complete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

Graphics* Graphics::getInstance() {
    if(_graphicsInstance == nullptr) {
        _graphicsInstance = new Graphics();
    }
    return _graphicsInstance;
}
