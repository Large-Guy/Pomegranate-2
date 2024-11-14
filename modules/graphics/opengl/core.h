#ifndef POMEGRANATEENGINE_CORE_H
#define POMEGRANATEENGINE_CORE_H
#include <core/core.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window;

class Graphics {
private:
    static Graphics _instance;
    static std::vector<Window*> _windows;
public:
    static int _bufferBindingCount;

    Graphics();

    static Graphics* getInstance();

    friend Window;
};


#endif //POMEGRANATEENGINE_CORE_H
