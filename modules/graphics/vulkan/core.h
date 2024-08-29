#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class Graphics {
private:
    static VkInstance instance;
public:
    static const char* getAPI();
    static void init();
    static void terminate();
};


#endif //POMEGRANATEENGINE_CORE_H
