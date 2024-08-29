#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
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
public:
    PomegranateWindow();
    ~PomegranateWindow();
    void show();
    void hide();
    void setTitle(const std::string& new_title);
    void setSize(int new_width, int new_height);
    void setFullscreen(bool is_fullscreen);
    void poll();
    bool is_open();
};


#endif //POMEGRANATEENGINE_WINDOW_H
