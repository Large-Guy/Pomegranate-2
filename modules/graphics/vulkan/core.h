#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#ifdef __linux__
#include <vulkan/vulkan_xlib.h>
#endif

#include <iostream>
#include <vector>
#include <map>
#include <optional>
class Graphics {
public:
    struct QueueFamilies {
        std::optional<uint32_t> graphicsFamily;
        bool isComplete() const;
    };
    static VkInstance instance;
    static VkPhysicalDevice physicalDevice;
    static VkDevice logicalDevice;
    static VkQueue graphicsQueue;

    static bool isDeviceValid(VkPhysicalDevice device);
    static QueueFamilies getQueueFamilies(VkPhysicalDevice device);
public:
    static const char* getAPI();
    static void init();
    static void terminate();
};


#endif //POMEGRANATEENGINE_CORE_H
