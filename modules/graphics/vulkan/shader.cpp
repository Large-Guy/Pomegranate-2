#include "shader.h"

VkShaderModule ShaderBase::createShaderModule(const List<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    Debug::AssertIf::isFalse(vkCreateShaderModule(Graphics::getInstance()->_logicalDevice, &createInfo, nullptr, &shaderModule) == VK_SUCCESS, "Failed to create shader module!");
    return shaderModule;
}

void ShaderBase::requestPipeline(Window *window) {
    Graphics::GraphicsPipelineGroup group = Graphics::getInstance()->createGraphicsPipeline(_bindingDescription,_attributeDescriptions,this, window, _renderMode, _cullMode);
    _pipelines[window] = group;
}