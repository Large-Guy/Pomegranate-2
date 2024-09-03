#ifndef POMEGRANATE_ENGINE_GRAPHICS_VULKAN_CORE_H
#define POMEGRANATE_ENGINE_GRAPHICS_VULKAN_CORE_H
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif __linux__
#define VK_USE_PLATFORM_XLIB_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_xlib.h>
#elif __APPLE__
#define VK_USE_PLATFORM_MACOS_MVK
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
//Include the vulkan header file for macOS
#include <vulkan/vulkan_macos.h>
#endif

#include <core/core.h>
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

class Graphics {
public:
    struct QueueFamilies {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        bool isComplete() const;
    };

    struct Queues {
        VkQueue graphics;
        VkQueue present;
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    static std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    static VkInstance instance;
    static VkPhysicalDevice physicalDevice;
    static VkDevice logicalDevice;
    static Queues queues;
    static std::vector<const char*> deviceExtensions;
    static std::vector<const char*> validationLayers;

    static bool isDeviceValid(VkPhysicalDevice device, VkSurfaceKHR surface);
    static bool hasExtensionSupport(VkPhysicalDevice device);
    static QueueFamilies getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    static SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    static bool hasValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
public:
    static const char* getAPI();
    static void init();
    static void terminate();
};


#endif //POMEGRANATE_ENGINE_GRAPHICS_VULKAN_CORE_H
