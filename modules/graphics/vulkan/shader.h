#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>

class Shader {
private:
    VkShaderModule _fragment;
    VkShaderModule _vertex;

    std::unordered_map<Window*, Graphics::GraphicsPipelineGroup> _pipelines;

    VkShaderModule createShaderModule(const List<char>& code);
    void requestPipeline(Window* window);
public:
    Shader(List<char> vertex, List<char> fragment);
    ~Shader();
    friend Graphics;
    friend Window;
};


#endif //POMEGRANATEENGINE_SHADER_H
