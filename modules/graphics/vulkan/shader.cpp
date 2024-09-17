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
    std::vector<VkDescriptorSetLayoutBinding> bindings(uniforms.size());

    for(auto& uniform : uniforms)
    {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = uniform.binding;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        layoutBinding.pImmutableSamplers = nullptr;
        bindings[uniform.binding] = layoutBinding;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    Debug::AssertIf::isFalse(vkCreateDescriptorSetLayout(Graphics::getInstance()->_logicalDevice,&layoutInfo, nullptr,&descriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor layout!");
}

void ShaderBase::createUniformBuffers() {
    for(auto& uniform : uniforms) {
        VkDeviceSize bufferSize = uniform.size;

        uniform.buffers.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
        uniform.memory.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
        uniform.mapped.resize(Graphics::MAX_FRAMES_IN_FLIGHT);

        for (int i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; ++i) {
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniform.buffers[i],
                         uniform.memory[i]);
            vkMapMemory(Graphics::getInstance()->_logicalDevice, uniform.memory[i], 0, bufferSize, 0,
                        &uniform.mapped[i]);
        }
    }
}

void ShaderBase::createDescriptorPool() {
    std::vector<VkDescriptorPoolSize> poolSizes(uniforms.size());

    for(auto& uniform : uniforms) {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = Graphics::MAX_FRAMES_IN_FLIGHT;
        poolSizes[uniform.binding] = poolSize;
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = Graphics::MAX_FRAMES_IN_FLIGHT;

    Debug::AssertIf::isFalse(vkCreateDescriptorPool(Graphics::getInstance()->_logicalDevice, &poolInfo, nullptr, &descriptorPool) == VK_SUCCESS, "Failed to create descriptor pool!");
}

void ShaderBase::createDescriptorSets() {
    for(auto& uniform : uniforms) {
        std::vector<VkDescriptorSetLayout> layouts(Graphics::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = Graphics::MAX_FRAMES_IN_FLIGHT;
        allocInfo.pSetLayouts = layouts.data();

        uniform.descriptors.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
        Debug::AssertIf::isFalse(
                vkAllocateDescriptorSets(Graphics::getInstance()->_logicalDevice, &allocInfo, uniform.descriptors.data()) == VK_SUCCESS,
                "Failed to allocate descriptor sets!"
        );

        std::vector<VkWriteDescriptorSet> descriptorWrites(Graphics::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniform.buffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = uniform.size;  // Use the correct size for each uniform

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = uniform.descriptors[i];
            descriptorWrite.dstBinding = uniform.binding;  // Use the correct binding for each uniform
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            descriptorWrites[i] = descriptorWrite;
        }

        vkUpdateDescriptorSets(Graphics::getInstance()->_logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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

    memcpy(uniforms[0].mapped[currentImage],&perspective,sizeof(perspective));

    Material material{};
    material.albedo = {1.0, 0.0, 0.0};
    memcpy(uniforms[1].mapped[currentImage],&material,sizeof(material));
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