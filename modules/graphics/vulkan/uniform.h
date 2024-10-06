#ifndef POMEGRANATEENGINE_UNIFORM_H
#define POMEGRANATEENGINE_UNIFORM_H
#include "core.h"
#include "vulkan/vulkan.h"

struct Uniform {
    size_t binding;
    size_t size;

    Uniform();
    Uniform(size_t binding,size_t size);
};

struct UniformBuffer {
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> mappedMemory;
    Uniform uniform;
    UniformBuffer(const Uniform& uniform);
    ~UniformBuffer();
    void createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void createUniformBuffers();
};

struct DescriptorSet {
    std::vector<Uniform> uniforms;
    std::vector<UniformBuffer> uniformBuffers;

    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

    DescriptorSet();
    DescriptorSet(std::vector<Uniform> uniforms, VkShaderStageFlagBits type);
    ~DescriptorSet();
    void createDescriptorSetLayout(VkShaderStageFlagBits type);
};


#endif //POMEGRANATEENGINE_UNIFORM_H
