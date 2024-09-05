#include "shader.h"

VkShaderModule Shader::createShaderModule(const List<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size() * sizeof(uint32_t);
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    Debug::AssertIf::isFalse(vkCreateShaderModule(Graphics::getInstance()->_logicalDevice, &createInfo, nullptr, &shaderModule) == VK_SUCCESS, "Failed to create shader module!");
    return shaderModule;
}

Shader::Shader(List<char> vertex, List<char> fragment) {
    _vertex = createShaderModule(vertex);
    _fragment = createShaderModule(fragment);

    Graphics::getInstance()->_shaders.push_back(this);

    Debug::Log::pass("Successfully compiled shaders!");
}

Shader::~Shader() {
    vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _vertex, nullptr);
    vkDestroyShaderModule(Graphics::getInstance()->_logicalDevice, _fragment, nullptr);
}