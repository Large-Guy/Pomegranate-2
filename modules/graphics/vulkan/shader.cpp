#include "shader.h"

VkShaderModule Shader::createShaderModule(std::vector<char> code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if(vkCreateShaderModule(Graphics::logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module!");
    }

    return shaderModule;
}

Shader::Shader(std::vector<char> vertex_shader, std::vector<char> fragment_shader) {

    _vertexModule = createShaderModule(vertex_shader);
    _fragmentModule = createShaderModule(fragment_shader);

    //Create pipeline stages
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = _vertexModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    vertShaderStageInfo.module = _fragmentModule;
    vertShaderStageInfo.pName = "main";

    Graphics::shaderStages.push_back(vertShaderStageInfo);
    Graphics::shaderStages.push_back(fragShaderStageInfo);
}

Shader::~Shader()
{
    vkDestroyShaderModule(Graphics::logicalDevice, _vertexModule, nullptr);
    vkDestroyShaderModule(Graphics::logicalDevice, _fragmentModule, nullptr);
}

const String &Shader::getVertexShader() const {
    return _vertexShader;
}

const String &Shader::getFragmentShader() const {
    return _fragmentShader;
}
