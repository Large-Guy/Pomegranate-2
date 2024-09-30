#ifndef POMEGRANATEENGINE_SHADER_H
#define POMEGRANATEENGINE_SHADER_H
#include "core.h"
#include <core/core.h>
#include "enumerations.h"
#include "vertex3d.h"
#include "math/math.h"

struct Perspective {
    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
};

struct Material {
    Vector3 albedo;
};



class ShaderBase{
private:
    void createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage,VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
public:
    VkShaderModule _fragment;
    VkShaderModule _vertex;
    RenderInfo _info = {
            .renderMode = RENDER_MODE_FILL,
            .cullMode = CULL_MODE_BACK,
            .topologyMode = TOPOLOGY_MODE_TRIANGLE_INDEXED
    };
    VkVertexInputBindingDescription _bindingDescription;
    std::vector<VkVertexInputAttributeDescription> _attributeDescriptions;

    std::unordered_map<Window*, Graphics::GraphicsPipelineGroup> _pipelines;

    VkShaderModule createShaderModule(const List<char>& code);
    void requestPipeline(Window* window);

    virtual ~ShaderBase() = default;

    friend Graphics;
};

template <typename VertexType>
class Shader : public ShaderBase{
public:
    Shader(List<char> vertex, List<char> fragment, RenderInfo info = {.renderMode = RENDER_MODE_FILL, .cullMode = CULL_MODE_BACK})
    {
        _bindingDescription = VertexType::getBindingDescription();
        _attributeDescriptions = VertexType::getAttributeDescriptions();

        _info = info;

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
        for (auto window: Graphics::getInstance()->_windows) {
            vkDestroyPipeline(Graphics::getInstance()->_logicalDevice, _pipelines[window].pipeline, nullptr);
            vkDestroyPipelineLayout(Graphics::getInstance()->_logicalDevice, _pipelines[window].layout, nullptr);
        }
        //Remove from shaders list
        Graphics::getInstance()->_shaders.erase(
                std::remove(Graphics::getInstance()->_shaders.begin(), Graphics::getInstance()->_shaders.end(), this),
                Graphics::getInstance()->_shaders.end());

    }
    friend Graphics;
    friend Window;
};


#endif //POMEGRANATEENGINE_SHADER_H
