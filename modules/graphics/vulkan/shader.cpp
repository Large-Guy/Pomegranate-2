#include "shader.h"

void ShaderBase::createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    Debug::AssertIf::isFalse(vkCreateBuffer(Graphics::getInstance()->_logicalDevice, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "Failed to create buffer!");
    Debug::Log::pass("Successfully created buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Graphics::getInstance()->_logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Graphics::getInstance()->getMemoryType(memRequirements.memoryTypeBits, properties);

    Debug::AssertIf::isFalse(vkAllocateMemory(Graphics::getInstance()->_logicalDevice, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "Failed to allocate buffer memory!");
    Debug::Log::pass("Successfully allocated buffer memory!");

    vkBindBufferMemory(Graphics::getInstance()->_logicalDevice, buffer, bufferMemory, 0);
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

    Graphics::GraphicsPipelineGroup group = Graphics::getInstance()->createGraphicsPipeline(this, window, _info,_bindingDescription,_attributeDescriptions);
    _pipelines[window] = group;
}