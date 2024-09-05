#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>

class Shader {
private:
    VkShaderModule _fragment;
    VkShaderModule _vertex;

    VkShaderModule createShaderModule(const List<char>& code);
public:
    Shader(List<char> vertex, List<char> fragment);
    ~Shader();
};


#endif //POMEGRANATEENGINE_SHADER_H
