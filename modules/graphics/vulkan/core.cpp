#include "core.h"

VkInstance Graphics::_instance = VK_NULL_HANDLE;
VkPhysicalDevice Graphics::_physicalDevice = VK_NULL_HANDLE;
VkDevice Graphics::_logicalDevice = VK_NULL_HANDLE;
Graphics::Queues Graphics::_queues;
std::vector<const char*> Graphics::validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

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

    Debug::Log::info("Available extensions:");

    for (const auto& extension : extensions) {
        Debug::Log::info(String("    ") + extension.extensionName);
    }

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
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance,&deviceCount, nullptr);

    Debug::AssertIf::isZero(deviceCount,"Unable to find GPU(s) with Vulkan support!");
    Debug::Log::info(String("Found ") + String((int)deviceCount) + " devices with Vulkan support");

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
    Debug::Log::info(String("Found suitable GPU - ") + deviceProperties.deviceName);
}

void Graphics::createLogicalDevice(bool enableValidationLayers) {
    QueueFamilyIndices indices = getQueueFamilies(_physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

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
    Debug::Log::info("Created logical device.");

    vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0, &_queues.graphicsQueue);
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

bool Graphics::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = getQueueFamilies(device);

    return indices.complete();
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

        if(indices.complete())
            break;

        i++;
    }

    return indices;
}

void Graphics::init(bool enableValidationLayers) {
    Debug::AssertIf::isFalse(glfwInit(), "Failed to initialize GLFW");
    Graphics::createInstance(enableValidationLayers);
    Graphics::createPhysicalDevice();
    Graphics::createLogicalDevice(enableValidationLayers);
}

void Graphics::destroy() {
    vkDestroyDevice(_logicalDevice, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwTerminate();
}

bool Graphics::QueueFamilyIndices::complete() {
    return graphicsFamily.has_value();
}
