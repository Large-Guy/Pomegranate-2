#ifndef POMEGRANATE_ENGINE_GRAPHICS_VULKAN_WINDOW_H
#define POMEGRANATE_ENGINE_GRAPHICS_VULKAN_WINDOW_H
#include "core.h"

class PomegranateWindow {
private:
    int width, height;
    bool fullscreen;
    std::string title;
    GLFWwindow* window;
    bool visible;
    bool close_requested;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
public:
    PomegranateWindow();
    ~PomegranateWindow();
    void show();
    void hide();
    void setTitle(const std::string& new_title);
    void setSize(int new_width, int new_height);
    void setFullscreen(bool is_fullscreen);
    void poll();
    bool isOpen();
};


#endif //POMEGRANATE_ENGINE_GRAPHICS_VULKAN_WINDOW_H
