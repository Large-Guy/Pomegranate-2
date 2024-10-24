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
    Uniform uniform;
    std::vector<VkBuffer> buffer;
    std::vector<VkDeviceMemory> memory;
    std::vector<void*> mapped;
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

    template <typename T>
    void set(int frame,size_t binding, T& data) {
        for (size_t i = 0; i < uniforms.size(); i++) {
            if (uniforms[i].binding == binding) {
                memcpy(uniformBuffers[i].mapped[frame], &data, sizeof(T));
            }
        }
    }

    DescriptorSet();
    DescriptorSet(std::vector<Uniform> uniforms, VkShaderStageFlagBits type);
    ~DescriptorSet();
    void createDescriptorSetLayout(VkShaderStageFlagBits type);
};


#endif //POMEGRANATEENGINE_UNIFORM_H
