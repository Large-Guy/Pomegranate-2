#include "shader.h"

VkShaderModule Shader::createShaderModule(const List<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    Debug::AssertIf::isFalse(vkCreateShaderModule(Graphics::getInstance()->_logicalDevice, &createInfo, nullptr, &shaderModule) == VK_SUCCESS, "Failed to create shader module!");
    return shaderModule;
}

void Shader::requestPipeline(Window *window) {
    Graphics::GraphicsPipelineGroup group = Graphics::getInstance()->createGraphicsPipeline(this, window);
    _pipelines[window] = group;
}

Shader::Shader(List<char> vertex, List<char> fragment) {
    _vertex = createShaderModule(vertex);
    _fragment = createShaderModule(fragment);

    Graphics::getInstance()->_shaders.push_back(this);

    Debug::Log::pass("Successfully compiled shaders!");

    for(auto window : Graphics::getInstance()->_windows) {
        requestPipeline(window);
    }
}

Shader::~Shader() {
    vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _vertex, nullptr);
    vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _fragment, nullptr);
    for(auto window : Graphics::getInstance()->_windows) {
        vkDestroyPipeline(Graphics::getInstance()->_logicalDevice, _pipelines[window].pipeline, nullptr);
        vkDestroyPipelineLayout(Graphics::getInstance()->_logicalDevice, _pipelines[window].layout, nullptr);
    }
    //Remove from shaders list
    Graphics::getInstance()->_shaders.erase(std::remove(Graphics::getInstance()->_shaders.begin(), Graphics::getInstance()->_shaders.end(), this), Graphics::getInstance()->_shaders.end());
}