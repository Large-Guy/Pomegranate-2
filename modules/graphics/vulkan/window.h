#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include "core.h"
#include <core/core.h>
#include <math/math.h>

class Window {
private:
    GLFWwindow* _window;
    VkSurfaceKHR _surface;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapExtent;


    String _title;
    Vector2i _size;
    bool _fullscreen;
    bool _visible;
    bool _open;
    Vector2i _position;

    void createSwapChain();
    VkExtent2D getExtents(const VkSurfaceCapabilitiesKHR& capabilities);
public:
    Window();
    ~Window();
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
};


#endif //POMEGRANATEENGINE_WINDOW_H
