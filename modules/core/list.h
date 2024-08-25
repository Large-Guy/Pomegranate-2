#ifndef POMEGRANATEENGINE_LIST_H
#define POMEGRANATEENGINE_LIST_H
#include "serializable.h"
#include <iterator>

template <typename T>
struct List : public Serializable {
private:
    T* elements;
    size_t count;
    size_t capacity;

    inline void expandCapacity()
    {
        if(capacity == 0)
        {
            capacity = 1;
        }
        else
        {
            capacity *= 2;
        }
        if(elements == nullptr)
        {
            elements = new T[capacity];
        }
        else
        {
            T* new_elements = new T[capacity];
            for(size_t i = 0; i < count; i++)
            {
                new_elements[i] = elements[i];
            }
            delete[] elements;
            elements = new_elements;
        }
    }

    inline void shrinkCapacity()
    {
        capacity /= 2;
        T* new_elements = new T[capacity];
        for(size_t i = 0; i < count; i++)
        {
            new_elements[i] = elements[i];
        }
        delete[] elements;
        elements = new_elements;
    }

public:

    List()
    {
        this->elements = nullptr;
        this->count = 0;
        this->capacity = 0;
    }

    List(size_t capacity)
    {
        this->elements = nullptr;
        this->count = 0;
        this->capacity = 0;
        resize(capacity);
    }

    List(const List<T>& other)
    {
        this->elements = nullptr;
        this->count = 0;
        this->capacity = 0;
        resize(other.capacity);
        for(size_t i = 0; i < other.count; i++)
        {
            elements[i] = other.elements[i];
        }
        count = other.count;
    }

    List(List<T>&& other) noexcept
    {
        this->elements = other.elements;
        this->count = other.count;
        this->capacity = other.capacity;
        other.elements = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    ~List()
    {
        delete[] elements;
    }

    List<T>& operator=(const List<T>& other)
    {
        if(this == &other)
        {
            return *this;
        }
        clear();
        resize(other.capacity);
        for(size_t i = 0; i < other.count; i++)
        {
            elements[i] = other.elements[i];
        }
        count = other.count;
        return *this;
    }

    List<T>& operator=(List<T>&& other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        clear();
        this->elements = other.elements;
        this->count = other.count;
        this->capacity = other.capacity;
        other.elements = nullptr;
        other.count = 0;
        other.capacity = 0;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const List<T>& list)
    {
        os << "[";
        for(size_t i = 0; i < list.count; i++)
        {
            os << list.elements[i];
            if(i != list.count - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    inline bool inBounds(int i)
    {
        if(i >= count || i < 0)
        {
            return false;
        }
        return true;
    }

    void add(T value)
    {
        if(count >= capacity)
        {
            expandCapacity();
        }
        elements[count++] = value;
    }

    void remove(int idx)
    {
        if(!inBounds(idx))
        {
            throw std::runtime_error("Outside bounds!");
        }

        for(int i = idx+1; i < count; i++)
        {
            elements[i-1] = elements[i];
        }
        count--;
        if(count <= capacity / 2)
        {
            shrinkCapacity();
        }
    }

    void remove(int idx, int c)
    {
        if(!inBounds(idx))
        {
            throw std::runtime_error("Outside bounds!");
        }

        for(int i = idx+1; i < count; i++)
        {
            elements[i-c] = elements[i];
        }
        count -= c;
        if(count < capacity / 2)
        {
            shrinkCapacity();
        }
    }

    size_t size()
    {
        return count;
    }

    void clear()
    {
        delete[] elements;
        elements = nullptr;
        count = 0;
        capacity = 0;
    }

    void resize(size_t new_size)
    {
        if(new_size < count)
        {
            count = new_size;
            if(count < capacity / 2)
            {
                shrinkCapacity();
            }
        }
        else if(new_size > count)
        {
            while(new_size > capacity)
            {
                expandCapacity();
            }
            count = new_size;
        }
    }

    T& operator[](int i)
    {
        if(!inBounds(i))
        {
            throw std::runtime_error("Outside bounds!");
        }
        return elements[i];
    }

    //Iterator
    class Iterator
    {
    private:
        T* ptr;
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        Iterator& operator++()
        {
            ptr++;
            return *this;
        }
        bool operator!=(const Iterator& other) const
        {
            return ptr != other.ptr;
        }
        T& operator*() const
        {
            return *ptr;
        }
    };

    Iterator begin()
    {
        return Iterator(elements);
    }

    Iterator end()
    {
        return Iterator(elements + count);
    }

    T* data()
    {
        return elements;
    }

    //Serialization

    void serialize(Archive& archive) const override
    {
        archive << count;
        for(size_t i = 0; i < count; i++) {
            archive << elements[i];
        }
    }

    void deserialize(Archive& archive) override
    {
        size_t c;
        archive >> &c;
        resize(c);
        for(size_t i = 0; i < count; i++) {
            archive >> &elements[i];
        }
    }
};


#endif //POMEGRANATEENGINE_LIST_H
