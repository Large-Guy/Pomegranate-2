#ifndef POMEGRANATEENGINE_BUFFER_H
#define POMEGRANATEENGINE_BUFFER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "enumerations.h"
#include <core/core.h>
#include "core.h"
#include "window.h"

template<BufferType Type>
class BufferBase {
protected:

    GLuint _buffer;
    GLuint _array;

    virtual void generate(const void* data) = 0;

public:
    GLsizei size;

    BufferBase()
    {
        size = 0;
        _buffer = 0;
        _array = 0;
    }
    ~BufferBase()
    {
        if(_buffer != 0)
        {
            glDeleteBuffers(1, &_buffer);
        }

    }

    void bind(Window* window)
    {
        switch(Type)
        {
            case BUFFER_TYPE_VERTEX: {
                glBindVertexArray(_array);
                break;
            }
            case BUFFER_TYPE_INDEX: {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
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

    void generate(const void* data) override{
        if constexpr (Type == BUFFER_TYPE_VERTEX)
        {
            VertexBindingInfo bindingInfo = T::getBindingInfo();
            glGenVertexArrays(1, &this->_array);
            glGenBuffers(1, &this->_buffer);

            glBindVertexArray(this->_array);

            glBindBuffer(GL_ARRAY_BUFFER, this->_buffer);
            glBufferData(GL_ARRAY_BUFFER, this->size * sizeof(T), data, GL_STATIC_DRAW);

            std::vector<VertexAttributeInfo> attributeDescriptions = T::getAttributeInfo();

            for (int i = 0; i < attributeDescriptions.size(); i++)
            {
                AttributeFormat format = attributeDescriptions[i].format;
                switch (format) {
                    case ATTRIBUTE_FORMAT_FLOAT:
                        glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_FLOAT2:
                        glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_FLOAT3:
                        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_FLOAT4:
                        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_INT:
                        glVertexAttribIPointer(i, 1, GL_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_INT2:
                        glVertexAttribIPointer(i, 2, GL_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_INT3:
                        glVertexAttribIPointer(i, 3, GL_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_INT4:
                        glVertexAttribIPointer(i, 4, GL_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_UNSIGNED_INT:
                        glVertexAttribIPointer(i, 1, GL_UNSIGNED_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_UNSIGNED_INT2:
                        glVertexAttribIPointer(i, 2, GL_UNSIGNED_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_UNSIGNED_INT3:
                        glVertexAttribIPointer(i, 3, GL_UNSIGNED_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                    case ATTRIBUTE_FORMAT_UNSIGNED_INT4:
                        glVertexAttribIPointer(i, 4, GL_UNSIGNED_INT, bindingInfo.stride, (void*)attributeDescriptions[i].offset);
                        break;
                }
                glEnableVertexAttribArray(i);
            }
            Debug::Log::pass("Generated vertex buffer!");
        }
        else if constexpr (Type == BUFFER_TYPE_INDEX)
        {
            glGenBuffers(1, &this->_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T), data, GL_STATIC_DRAW);
            Debug::Log::pass("Generated index buffer!");
        }
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
        generate(_data.data());
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
        if(this->_buffer != 0) {
            glDeleteBuffers(1, &this->_buffer);
        }

        generate(_data.data());
    }

    friend class Graphics;
    friend class Window;
};


#endif //POMEGRANATEENGINE_BUFFER_H
