#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include "core.h"

class Window {
private:
    int width, height;
    bool fullscreen;
    std::string title;
    GLFWwindow* window;
    bool visible;
    bool close_requested;
public:
    Window();
    ~Window();
    void show();
    void hide();
    void setTitle(const std::string& new_title);
    void setSize(int new_width, int new_height);
    void setFullscreen(bool is_fullscreen);
    void poll();
    bool is_open();
};


#endif //POMEGRANATEENGINE_WINDOW_H
