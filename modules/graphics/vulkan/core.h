#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#elif __linux__
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#elif __APPLE__
#define VK_USE_PLATFORM_MACOS_MVK
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
#include <core/core.h>

class Graphics {
private:
    static VkInstance _instance;
    static std::vector<const char*> validationLayers;

    static void createInstance(bool enableValidationLayers);
    static bool hasValidationLayerSupport();
public:
    static void init(bool enableValidationLayers = false);
    static void destroy();
};


#endif //POMEGRANATEENGINE_CORE_H
