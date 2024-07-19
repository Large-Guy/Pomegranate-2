#include "shader.h"

Shader::Shader(std::string vertexSource, std::string fragmentSource)
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

void Shader::compileShader(std::string source, GLenum type)
{
    const char* c = source.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);
    glAttachShader(id, shader);
    glDeleteShader(shader);
}

void Shader::set(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void Shader::set(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::set(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::set(const char *name, const Vector2 &value) const {
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

void Shader::set(const char *name, const Vector3 &value) const {
    glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}

void Shader::set(const char *name, const Vector4 &value) const {
    glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
}
