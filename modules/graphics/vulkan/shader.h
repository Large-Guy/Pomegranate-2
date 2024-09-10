#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>
#include "enumerations.h"

class Shader {
private:
    VkShaderModule _fragment;
    VkShaderModule _vertex;
    RenderMode _renderMode = Fill;
    CullMode _cullMode = Back;

    std::unordered_map<Window*, Graphics::GraphicsPipelineGroup> _pipelines;

    VkShaderModule createShaderModule(const List<char>& code);
    void requestPipeline(Window* window);
public:
    Shader(List<char> vertex, List<char> fragment, RenderInfo info = {.renderMode = Fill, .cullMode = Back});
    ~Shader();
    friend Graphics;
    friend Window;
};


#endif //POMEGRANATEENGINE_SHADER_H
