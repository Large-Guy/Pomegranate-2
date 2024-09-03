#ifndef POMEGRANATE_ENGINE_GRAPHICS_VULKAN_SHADER_H
#define POMEGRANATE_ENGINE_GRAPHICS_VULKAN_SHADER_H
#include <core/core.h>
#include "core.h"

class Shader {
    String _vertexShader;
    String _fragmentShader;
    VkShaderModule _vertexModule;
    VkShaderModule _fragmentModule;
    VkPipelineShaderStageCreateInfo stages[2] = {};
    VkShaderModule createShaderModule(std::vector<char> code);
public:
    Shader(std::vector<char> vertex_shader, std::vector<char> fragment_shader);
    ~Shader();
    const String& getVertexShader() const;
    const String& getFragmentShader() const;
};


#endif //POMEGRANATE_ENGINE_GRAPHICS_VULKAN_SHADER_H
