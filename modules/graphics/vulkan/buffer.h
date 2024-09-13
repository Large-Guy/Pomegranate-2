#ifndef POMEGRANATEENGINE_BUFFER_H
#define POMEGRANATEENGINE_BUFFER_H
#include <vulkan/vulkan.h>
#include <vector>
#include "enumerations.h"
#include <core/core.h>
#include "core.h"

template <typename T>
class Buffer {
private:
    std::vector<T> _data;
    BufferType _type;
    VkBuffer _buffer;
    VkDeviceMemory _memory;


    void createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
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

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandPool = Graphics::getInstance()->_commandPool;
        allocateInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Graphics::getInstance()->_logicalDevice,&allocateInfo,&commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer,srcBuffer,dstBuffer,1,&copyRegion);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(Graphics::getInstance()->_queues.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(Graphics::getInstance()->_queues.graphicsQueue);

        vkFreeCommandBuffers(Graphics::getInstance()->_logicalDevice,Graphics::getInstance()->_commandPool,1,&commandBuffer);
    }

    void generateBuffer()
    {
        VkDeviceSize bufferSize = sizeof(_data[0]) * _data.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory,0,bufferSize,0,&data);
        memcpy(data,_data.data(),(size_t)bufferSize);
        vkUnmapMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory);

        createBuffer(bufferSize, (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT | ((VkBufferUsageFlagBits)_type)), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _buffer, _memory);
        copyBuffer(stagingBuffer,_buffer,bufferSize);

        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice,stagingBuffer, nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory, nullptr);
    }
public:
    Buffer(std::vector<T> data, BufferType type)
    {
        _buffer = VK_NULL_HANDLE;
        _memory = VK_NULL_HANDLE;
        _data = data;
        _type = type;
        generateBuffer();
    }

    ~Buffer()
    {
        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice, _buffer, nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice, _memory, nullptr);
    }

    std::vector<T> data() const
    {
        return _data;
    }

    void add(T data)
    {
        _data.push_back(data);
    }

    void remove(int index)
    {
        _data.erase(_data.begin() + index);
    }

    void clear()
    {
        _data.clear();
    }

    void update(int index, T data)
    {
        _data[index] = data;
    }

    void set(std::vector<T> data)
    {
        _data = data;
    }

    void regenerate()
    {
        vkDeviceWaitIdle(Graphics::getInstance()->_logicalDevice);
        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice, _buffer, nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice, _memory, nullptr);

        generateBuffer();
    }

    friend class Graphics;
    friend class Window;
};


#endif //POMEGRANATEENGINE_BUFFER_H
