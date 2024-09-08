#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include "core.h"
#include <core/core.h>
#include <math/math.h>

class Window {
public:
    GLFWwindow* _window;
    VkSurfaceKHR _surface;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    std::vector<VkImageView> _swapChainImageViews;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapExtent;
    std::vector<VkFramebuffer> _swapChainFramebuffers;

    VkRenderPass _renderPass;

    VkCommandPool _commandPool;
    VkCommandBuffer _commandBuffer;

    String _title;
    Vector2i _size;
    bool _fullscreen;
    bool _visible;
    bool _open;
    Vector2i _position;

    void createSwapChain();
    void createImageViews();
    VkExtent2D getExtents(const VkSurfaceCapabilitiesKHR& capabilities);
public:
    Window();
    ~Window();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, Shader* shader);

    void setTitle(const String& title);
    void setSize(Vector2i size);
    void setSize(int width, int height);
    void poll();
    void show();
    void hide();
    void fullscreen();
    void maximize();
    void minimize();
    void focus();
    void setPosition(Vector2i position);
    void setPosition(int x, int y);

    Vector2i getPosition() const;
    Vector2i getSize() const;
    String getTitle() const;
    [[nodiscard]] bool isOpen() const;

    friend Graphics;
};


#endif //POMEGRANATEENGINE_WINDOW_H
