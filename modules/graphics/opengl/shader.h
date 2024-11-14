#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>
#include "enumerations.h"
#include "math/math.h"

class ShaderBase{
protected:
    GLuint _program;
    RenderInfo _info;
public:

    ShaderBase() = default;
    virtual ~ShaderBase() = default;

    void bind();

    friend Graphics;
    friend Window;
};

template <typename VertexType>
class Shader : public ShaderBase{
public:
    Shader(List<char> vertex, List<char> fragment, RenderInfo info = {.renderMode = RENDER_MODE_FILL, .cullMode = CULL_MODE_BACK})
    {
        _info = info;

        GLuint _vertex = glCreateShader(GL_VERTEX_SHADER);
        char* v = vertex.data();
        glShaderSource(_vertex, 1, &v, nullptr);
        glCompileShader(_vertex);

        //Check for errors
        GLint success;
        glGetShaderiv(_vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(_vertex, 512, nullptr, infoLog);
            Debug::Log::error("Vertex shader compilation failed: %s", infoLog);
        }

        GLuint _fragment = glCreateShader(GL_FRAGMENT_SHADER);
        char* f = fragment.data();
        glShaderSource(_fragment, 1, &f, nullptr);
        glCompileShader(_fragment);

        //Check for errors
        glGetShaderiv(_fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(_fragment, 512, nullptr, infoLog);
            Debug::Log::error("Fragment shader compilation failed: %s", infoLog);
        }

        _program = glCreateProgram();
        glAttachShader(_program, _vertex);
        glAttachShader(_program, _fragment);
        glLinkProgram(_program);

        //Check for errors
        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(_program, 512, nullptr, infoLog);
            Debug::Log::error("Shader program linking failed: %s", infoLog);
        }

        glDeleteShader(_vertex);
        glDeleteShader(_fragment);
    }
    ~Shader() override {
        glDeleteProgram(_program);
    }
    friend Graphics;
    friend Window;
};


#endif //POMEGRANATEENGINE_SHADER_H
