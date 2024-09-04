#include "core.h"

VkInstance Graphics::_instance = nullptr;
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

void Graphics::init(bool enableValidationLayers) {
    Debug::AssertIf::isFalse(glfwInit(), "Failed to initialize GLFW");
    Graphics::createInstance(enableValidationLayers);
}

void Graphics::destroy() {
    vkDestroyInstance(_instance, nullptr);
    glfwTerminate();
}