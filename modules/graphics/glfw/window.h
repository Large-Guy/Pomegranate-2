#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <model2d.h>
#include <shader.h>
#include <graphics_core.h>


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
        Model2D _rect;
        Shader _shader;
        void init();
    public:
        void begin();
        void end();
        void setColor(Vector4 color);
        void setColor(const Vector3& color, float a = 1.0);
        void clear() const;
        void drawTexture(Texture2D* texture, Vector2 position, Vector2 size, float rotation = 0.0f);


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
