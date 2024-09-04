#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include "core.h"
#include <core/core.h>
#include <math/math.h>

class Window {
private:
    GLFWwindow* _window;
    VkSurfaceKHR _surface;
    String _title;
    Vector2i _size;
    bool _fullscreen;
    bool _visible;
    bool _open;
    Vector2i _position;
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

    void destroy();
};


#endif //POMEGRANATEENGINE_WINDOW_H
