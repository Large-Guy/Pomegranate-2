#include "shader.h"

Shader::Shader(const char* vertexSource, const char* fragmentSource)
{
    this->vertexSource = vertexSource;
    this->fragmentSource = fragmentSource;
    id = glCreateProgram();
    compileShader(vertexSource, GL_VERTEX_SHADER);
    compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    glLinkProgram(id);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::compileShader(const char* source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glAttachShader(id, shader);
    glDeleteShader(shader);
}