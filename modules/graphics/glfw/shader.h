#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix2x2.h>
#include <matrix3x3.h>
#include <matrix4x4.h>
#include <texture.h>

class Shader {
private:
    GLuint id;
    std::string vertexSource;
    std::string fragmentSource;
    void compileShader(const std::string& source, GLenum type);
public:
    Shader();
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    void use() const;
    void set(const char* name, bool value) const;
    void set(const char* name, int value) const;
    void set(const char* name, float value) const;
    void set(const char* name, const Vector2& value) const;
    void set(const char* name, const Vector3& value) const;
    void set(const char* name, const Vector4& value) const;
    void set(const char* name, const Matrix2x2& value) const;
    void set(const char* name, const Matrix3x3& value) const;
    void set(const char* name, const Matrix4x4& value) const;
    void set(const char* name, const Texture2D& value) const;
};


#endif //POMEGRANATEENGINE_SHADER_H
