#include "uniform.h"

#include <utility>

Uniform::Uniform() {
    this->binding = 0;
    this->size = 0;
}

Uniform::Uniform(size_t binding, size_t size) {
    this->binding = binding;
    this->size = size;
}

UniformBuffer::UniformBuffer(const Uniform& uniform) {
    this->uniform = uniform;
    createUniformBuffers();
}

UniformBuffer::~UniformBuffer() {
    for (size_t i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice, uniformBuffers[i], nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice, uniformBuffersMemory[i], nullptr);
    }
}

void UniformBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags properties,
                           VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    Debug::AssertIf::isFalse(vkCreateBuffer(Graphics::getInstance()->_logicalDevice, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "Failed to create buffer!");
    Debug::Log::pass("Successfully created uniform buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Graphics::getInstance()->_logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Graphics::getInstance()->getMemoryType(memRequirements.memoryTypeBits, properties);

    Debug::AssertIf::isFalse(vkAllocateMemory(Graphics::getInstance()->_logicalDevice, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "Failed to allocate buffer memory!");
    Debug::Log::pass("Successfully allocated uniform buffer memory!");

    vkBindBufferMemory(Graphics::getInstance()->_logicalDevice, buffer, bufferMemory, 0);
}

void UniformBuffer::createUniformBuffers() {
    VkDeviceSize bufferSize = uniform.size;

    uniformBuffers.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(Graphics::MAX_FRAMES_IN_FLIGHT);
    mappedMemory.resize(Graphics::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Graphics::MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(Graphics::getInstance()->_logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &mappedMemory[i]);
    }
}

DescriptorSet::DescriptorSet() {
    this->uniforms = {};
}

DescriptorSet::DescriptorSet(std::vector<Uniform> uniforms, VkShaderStageFlagBits type) {
    this->uniforms = std::move(uniforms);

    //Initialize uniform buffers
    for(auto& uniform : this->uniforms) {
        uniformBuffers.emplace_back(uniform);
    }

    createDescriptorSetLayout(type);
}

DescriptorSet::~DescriptorSet() {
    vkDestroyDescriptorSetLayout(Graphics::getInstance()->_logicalDevice, descriptorSetLayout, nullptr);
}

void DescriptorSet::createDescriptorSetLayout(VkShaderStageFlagBits type) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for(auto& uniform : uniforms) {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = uniform.binding;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = type;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        bindings.push_back(uboLayoutBinding);

    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    Debug::AssertIf::isFalse(vkCreateDescriptorSetLayout(Graphics::getInstance()->_logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor set layout!");
    Debug::Log::pass("Successfully created descriptor set layout!");
}