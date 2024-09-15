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
    VkDeviceSize bufferSize = sizeof(Perspective);

    uniformBuffers.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(Graphics::MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; ++i) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        vkMapMemory(Graphics::getInstance()->_logicalDevice,uniformBuffersMemory[i],0,bufferSize,0,&uniformBuffersMapped[i]);
    }
}

void ShaderBase::createDescriptorPool() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(Graphics::MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(Graphics::MAX_FRAMES_IN_FLIGHT);

    Debug::AssertIf::isFalse(vkCreateDescriptorPool(Graphics::getInstance()->_logicalDevice, &poolInfo, nullptr, &descriptorPool) == VK_SUCCESS, "Failed to create descriptor pool!");
}

void ShaderBase::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(Graphics::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(Graphics::MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
    Debug::AssertIf::isFalse(vkAllocateDescriptorSets(Graphics::getInstance()->_logicalDevice,&allocInfo, descriptorSets.data()) == VK_SUCCESS, "Failed to allocate descriptor sets!");

    for(size_t i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(Perspective);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(Graphics::getInstance()->_logicalDevice,1,&descriptorWrite,0, nullptr);
    }
}

void ShaderBase::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    Perspective perspective{};
    perspective.model = {cosf(time), -sinf(time), 0.0, 0.0,
                         sinf(time), cosf(time), 0.0f, 0.0,
                         0.0,  0.0f, 1.0f, 0.0,
                         0.0, 0.0, 0.0, 1.0};
    perspective.view = {1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};
    perspective.projection = {2.0f / 4.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 2.0f/3.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};

    memcpy(uniformBuffersMapped[currentImage],&perspective,sizeof(perspective));
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

    Graphics::GraphicsPipelineGroup group = Graphics::getInstance()->createGraphicsPipeline(this, window, _info,_bindingDescription,_attributeDescriptions,&descriptorSetLayout);
    _pipelines[window] = group;
}