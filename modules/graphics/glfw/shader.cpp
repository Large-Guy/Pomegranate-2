#include "shader.h"

Shader::Shader() {
    this->vertexSource = "";
    this->fragmentSource = "";
    id = 0;
}

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
    //Create a shader program
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    //Load from file
    std::ifstream vertexShaderFile("assets/shaders/default2d/vertex.glsl");
    std::ifstream fragmentShaderFile("assets/shaders/default2d/fragment.glsl");

    if (vertexShaderFile.is_open()) {
        std::string line;
        while (getline(vertexShaderFile, line)) {
            vertexShaderSource += line + "\n";
        }
        vertexShaderFile.close();
    }

    if (fragmentShaderFile.is_open()) {
        std::string line;
        while (getline(fragmentShaderFile, line)) {
            fragmentShaderSource += line + "\n";
        }
        fragmentShaderFile.close();
    }

    this->vertexSource = vertexShaderSource;
    this->fragmentSource = fragmentShaderSource;
    id = glCreateProgram();
    compileShader(vertexSource, GL_VERTEX_SHADER);
    compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    glLinkProgram(id);
}

Shader::~Shader()
{
    if(id != 0)
        glDeleteProgram(id);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::compileShader(const std::string& source, GLenum type)
{
    const char* c = source.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<GLchar> infoLog(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog.data());
        std::cerr << "Shader compilation failed:\n" << infoLog.data() << std::endl;
        // Handle compilation failure, e.g., delete shader and exit
    }
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

void Shader::set(const char *name, const Matrix2x2 &value) const {
    //Convert Matrix2x2 to float array
    float values[4] = {value.x.x, value.x.y, value.y.x, value.y.y};
    glUniformMatrix2fv(glGetUniformLocation(id, name), 1, GL_FALSE, values);
}

void Shader::set(const char *name, const Matrix3x3 &value) const {
    //Convert Matrix3x3 to float array
    float values[9] = {value.x.x, value.x.y, value.x.z, value.y.x, value.y.y, value.y.z, value.z.x, value.z.y, value.z.z};
    glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, values);
}

void Shader::set(const char *name, const Matrix4x4 &value) const {
    //Convert Matrix4x4 to float array
    float values[16] = {value.x.x, value.x.y, value.x.z, value.x.w, value.y.x, value.y.y, value.y.z, value.y.w, value.z.x, value.z.y, value.z.z, value.z.w, value.w.x, value.w.y, value.w.z, value.w.w};
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, values);
}

void Shader::set(const char *name, const Texture2D &value) const {
    glUniform1i(glGetUniformLocation(id, name), value.getBindSlot());
}
