#ifndef POMEGRANATEENGINE_WINDOW_H
#define POMEGRANATEENGINE_WINDOW_H
#include <core/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math/math.h>
#include "core.h"
#include "mesh.h"
#include "shader.h"

class Window {
public:
    class Draw {
    private:
        TopologyMode _topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED;
        RenderInfo _info;
    public:
        Window* window;
        uint32_t imageIndex;
        void begin();
        void end();
        void clear(Vector4 color);
        //void buffers(BufferBase<BUFFER_TYPE_VERTEX>* vertexBuffer, BufferBase<BUFFER_TYPE_INDEX>* indexBuffer, ShaderBase* shader);
        void shader(ShaderBase* shader);
        void mesh(MeshBase* mesh);
    };

    GLFWwindow* _window;

    uint32_t _currentFrame = 0;

    std::string _title;
    Vector2i _size;
    bool _fullscreen;
    bool _visible;
    bool _open;
    Vector2i _position;

public:
    Draw draw;
    Window();
    ~Window();

    void setTitle(const std::string& title);
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
    std::string getTitle() const;
    [[nodiscard]] bool isOpen() const;

    friend Graphics;
};


#endif //POMEGRANATEENGINE_WINDOW_H
