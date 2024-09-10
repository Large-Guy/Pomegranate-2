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


    void generateBuffer()
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(_data[0]) * _data.size();
        bufferInfo.usage = (VkBufferUsageFlagBits)_type;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        Debug::AssertIf::isFalse(vkCreateBuffer(Graphics::getInstance()->_logicalDevice, &bufferInfo, nullptr, &_buffer) == VK_SUCCESS, "Failed to create buffer!");
        Debug::Log::pass("Successfully created buffer!");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(Graphics::getInstance()->_logicalDevice, _buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = Graphics::getInstance()->getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        Debug::AssertIf::isFalse(vkAllocateMemory(Graphics::getInstance()->_logicalDevice,&allocInfo,nullptr,&_memory) == VK_SUCCESS,"Failed to allocate buffer memory!");
        Debug::Log::pass("Successfully allocated buffer memory!");

        vkBindBufferMemory(Graphics::getInstance()->_logicalDevice,_buffer,_memory,0);

        void* data;
        vkMapMemory(Graphics::getInstance()->_logicalDevice,_memory,0,bufferInfo.size,0,&data);
        memcpy(data,_data.data(),(size_t)bufferInfo.size);
        vkUnmapMemory(Graphics::getInstance()->_logicalDevice,_memory);
    }
public:
    Buffer(std::vector<T> data, BufferType type)
    {
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
