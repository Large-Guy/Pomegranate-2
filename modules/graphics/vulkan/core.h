#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_beta.h>
#elif __linux__
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_beta.h>
#elif __APPLE__
#define VK_USE_PLATFORM_MACOS_MVK
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_beta.h>
#endif
#include <core/core.h>
#include <optional>
#include <set>

struct Window;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool complete();
};

class Graphics {
private:


    struct Queues {
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };

    VkInstance _instance;
    VkPhysicalDevice _physicalDevice;
    VkDevice _logicalDevice;
    Queues _queues;
    std::vector<const char*> validationLayers;
    VkSurfaceKHR _currentSurface;

    std::vector<const char*> deviceExtensions;

    std::vector<Window*> _windows;

    void createInstance(bool enableValidationLayers);
    void createPhysicalDevice();
    void createLogicalDevice(bool enableValidationLayers);
    bool hasValidationLayerSupport();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool hasDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR getSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

    static Graphics* _graphicsInstance;
public:
    static bool enableValidationLayers;
    Graphics();
    ~Graphics();

    static Graphics* getInstance();

    friend class Window;
};


#endif //POMEGRANATEENGINE_CORE_H
