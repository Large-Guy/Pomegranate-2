#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>
#include "enumerations.h"
#include "vertex3d.h"

class ShaderBase{
public:
    VkShaderModule _fragment;
    VkShaderModule _vertex;
    RenderMode _renderMode = Fill;
    CullMode _cullMode = Back;
    VkVertexInputBindingDescription _bindingDescription;
    std::vector<VkVertexInputAttributeDescription> _attributeDescriptions;

    std::unordered_map<Window*, Graphics::GraphicsPipelineGroup> _pipelines;

    VkShaderModule createShaderModule(const List<char>& code);
    void requestPipeline(Window* window);

    friend Graphics;
};

template <typename VertexType>
class Shader : public ShaderBase{
public:
    Shader(List<char> vertex, List<char> fragment, RenderInfo info = {.renderMode = Fill, .cullMode = Back})
    {
        _bindingDescription = VertexType::getBindingDescription();
        _attributeDescriptions = VertexType::getAttributeDescriptions();

        _renderMode = info.renderMode;
        _cullMode = info.cullMode;
        _vertex = createShaderModule(vertex);
        _fragment = createShaderModule(fragment);

        Graphics::getInstance()->_shaders.push_back(this);

        Debug::Log::pass("Successfully compiled shaders!");

        for(auto window : Graphics::getInstance()->_windows) {
            requestPipeline(window);
        }
    }
    ~Shader() {
        vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _vertex, nullptr);
        vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _fragment, nullptr);
        for(auto window : Graphics::getInstance()->_windows) {
            vkDestroyPipeline(Graphics::getInstance()->_logicalDevice, _pipelines[window].pipeline, nullptr);
            vkDestroyPipelineLayout(Graphics::getInstance()->_logicalDevice, _pipelines[window].layout, nullptr);
        }
        //Remove from shaders list
        Graphics::getInstance()->_shaders.erase(std::remove(Graphics::getInstance()->_shaders.begin(), Graphics::getInstance()->_shaders.end(), this), Graphics::getInstance()->_shaders.end());
    }
    friend Graphics;
    friend Window;
};


#endif //POMEGRANATEENGINE_SHADER_H
