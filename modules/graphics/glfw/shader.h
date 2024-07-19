#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
private:
    GLuint id;
    const char* vertexSource;
    const char* fragmentSource;
    void compileShader(const char* source, GLenum type);
public:
    Shader(const char* vertexSource, const char* fragmentSource);
    void use() const;
};


#endif //POMEGRANATEENGINE_SHADER_H
