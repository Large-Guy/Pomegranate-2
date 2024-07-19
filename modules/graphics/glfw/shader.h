#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>

class Shader {
private:
    GLuint id;
    std::string vertexSource;
    std::string fragmentSource;
    void compileShader(std::string source, GLenum type);
public:
    Shader(std::string vertexSource, std::string fragmentSource);
    void use() const;
    void set(const char* name, bool value) const;
    void set(const char* name, int value) const;
    void set(const char* name, float value) const;
    void set(const char* name, const Vector2& value) const;
    void set(const char* name, const Vector3& value) const;
    void set(const char* name, const Vector4& value) const;
};


#endif //POMEGRANATEENGINE_SHADER_H
