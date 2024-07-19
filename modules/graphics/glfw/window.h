#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>


class Window {
private:
    GLFWwindow* _window;
    int _width, _height;
    const char* _title;
    bool _open;
public:

    class Draw {
    private:
        Vector4 _color;
        GLFWwindow* _window;
    public:
        void begin();
        void end();
        void setColor(Vector4 color);
        void setColor(Vector3 color, float a = 1.0);
        void clear() const;
        void drawRect(float x, float y, float width, float height) const;


        friend class Window;
    };

    Draw draw;

    Window();
    ~Window();
    void setSize(int width, int height);
    void setTitle(const char* title);
    void open();
    void close();
    void pollEvents();
    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

};


#endif //POMEGRANATEENGINE_WINDOW_H
