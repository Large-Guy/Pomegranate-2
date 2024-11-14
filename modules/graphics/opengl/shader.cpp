#include "shader.h"

void ShaderBase::bind() {
    glUseProgram(_program);
}