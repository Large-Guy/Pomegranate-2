#include "shader.h"

void ShaderBase::createDescriptionSetLayout() {
    VkDescriptorSetLayoutBinding perspectiveLayoutBinding{};
    perspectiveLayoutBinding.binding = 0;
    perspectiveLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    perspectiveLayoutBinding.descriptorCount = 1;
    perspectiveLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    perspectiveLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &perspectiveLayoutBinding;

    Debug::AssertIf::isFalse(vkCreateDescriptorSetLayout(Graphics::getInstance()->_logicalDevice,&layoutInfo, nullptr,&descriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor layout!");
}

void ShaderBase::createUniformBuffers() {

}

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
    Graphics::GraphicsPipelineGroup group = Graphics::getInstance()->createGraphicsPipeline(this, window, _renderMode, _cullMode,_bindingDescription,_attributeDescriptions,&descriptorSetLayout);
    _pipelines[window] = group;
}