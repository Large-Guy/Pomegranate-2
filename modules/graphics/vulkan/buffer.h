#ifndef POMEGRANATEENGINE_BUFFER_H
#define POMEGRANATEENGINE_BUFFER_H
#include <vulkan/vulkan.h>
#include <vector>
#include "enumerations.h"
#include <core/core.h>
#include "core.h"
#include "window.h"

template<BufferType Type>
class BufferBase {
protected:
    VkBuffer _buffer;
    VkDeviceMemory _memory;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlagBits usage,VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage | ((VkBufferUsageFlagBits)Type);
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
public:
    VkDeviceSize size;

    BufferBase()
    {
        size = 0;
        _buffer = VK_NULL_HANDLE;
        _memory = VK_NULL_HANDLE;
    }

    void bind(Window* window)
    {
        switch(Type)
        {
            case BUFFER_TYPE_VERTEX: {
                VkBuffer vertexBuffers[] = {_buffer};
                VkDeviceSize offsets[] = {0};
                vkCmdBindVertexBuffers(window->getCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets);
                break;
            }
            case BUFFER_TYPE_INDEX: {
                vkCmdBindIndexBuffer(window->getCurrentCommandBuffer(),_buffer,0,VK_INDEX_TYPE_UINT16);
                break;
            }
            case BUFFER_TYPE_NONE: {
                Debug::Log::error("Unable to bind unknown type!");
                throw std::runtime_error("Unable to bind unknown type!");
            }
        }
    }
};

template <typename T, BufferType Type>
class Buffer : public BufferBase<Type> {
private:
    List<T> _data;

    void generateBuffer()
    {
        VkDeviceSize bufferSize = sizeof(_data[0]) * _data.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        BufferBase<Type>::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory,0,bufferSize,0,&data);
        memcpy(data,_data.data(),(size_t)bufferSize);
        vkUnmapMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory);

        BufferBase<Type>::createBuffer(bufferSize, (VkBufferUsageFlagBits)(VK_BUFFER_USAGE_TRANSFER_DST_BIT), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->_buffer, this->_memory);
        BufferBase<Type>::copyBuffer(stagingBuffer,this->_buffer,bufferSize);

        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice,stagingBuffer, nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice,stagingBufferMemory, nullptr);
    }
public:

    Buffer() : BufferBase<Type>()
    {
        _data = {};
        this->size = 0;
    }
    Buffer(List<T> data)  : BufferBase<Type>()
    {
        _data = data;
        this->size = data.size();
        generateBuffer();
    }

    ~Buffer()
    {
        vkDestroyBuffer(Graphics::getInstance()->_logicalDevice, this->_buffer, nullptr);
        vkFreeMemory(Graphics::getInstance()->_logicalDevice, this->_memory, nullptr);
    }

    List<T> data() const
    {
        return _data;
    }

    void add(T data)
    {
        _data.add(data);
        this->size += 1;
    }

    void remove(int index)
    {
        _data.remove(index);
        this->size -= 1;
    }

    void clear()
    {
        _data.clear();
        this->size = 0;
    }

    void update(int index, T data)
    {
        _data[index] = data;
    }

    void set(List<T> data)
    {
        _data = data;
        this->size = data.size();
    }

    void regenerate()
    {
        vkDeviceWaitIdle(Graphics::getInstance()->_logicalDevice);
        if(this->_buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(Graphics::getInstance()->_logicalDevice, this->_buffer, nullptr);
            vkFreeMemory(Graphics::getInstance()->_logicalDevice, this->_memory, nullptr);
        }

        generateBuffer();
    }

    friend class Graphics;
    friend class Window;
};


#endif //POMEGRANATEENGINE_BUFFER_H
