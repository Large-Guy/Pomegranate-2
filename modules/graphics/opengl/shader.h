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

    template <typename T>
    void setUniform(const std::string& name, T value) {
        glUseProgram(_program);

        GLint location = glGetUniformLocation(_program, name.c_str());
        if (location == -1) {
            Debug::Log::error("Uniform", name.c_str(), "not found");
            return;
        }

        if constexpr (std::is_same<T, int>::value) {
            glUniform1i(location, value);
        } else if constexpr (std::is_same<T, float>::value) {
            glUniform1f(location, value);
        } else if constexpr (std::is_same<T, Vector2>::value) {
            glUniform2f(location, value.x, value.y);
        } else if constexpr (std::is_same<T, Vector3>::value) {
            glUniform3f(location, value.x, value.y, value.z);
        } else if constexpr (std::is_same<T, Vector4>::value) {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        } else if constexpr (std::is_same<T, Matrix4x4>::value) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value.x.x);
        } else {
            Debug::Log::error("Uniform type not supported");
        }
    }

    ShaderBase() = default;
    virtual ~ShaderBase() = default;

    friend class Window;
};

template <typename VertexType>
class Shader : public ShaderBase{
public:
    Shader(const char* vertex, const char* fragment, RenderInfo info = {.renderMode = RENDER_MODE_FILL, .cullMode = CULL_MODE_BACK})
    {
        _info = info;

        GLuint _vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertex, 1, &vertex, nullptr);
        glCompileShader(_vertex);

        //Check for errors
        GLint success;
        glGetShaderiv(_vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(_vertex, 512, nullptr, infoLog);
            Debug::Log::error("Vertex shader compilation failed:", infoLog);
        }

        GLuint _fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(_fragment, 1, &fragment, nullptr);
        glCompileShader(_fragment);

        //Check for errors
        glGetShaderiv(_fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(_fragment, 512, nullptr, infoLog);
            Debug::Log::error("Fragment shader compilation failed:", infoLog);
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
            Debug::Log::error("Shader program linking failed:", infoLog);
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
