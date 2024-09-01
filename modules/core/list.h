#ifndef POMEGRANATE_ENGINE_CORE_LIST_H
#define POMEGRANATE_ENGINE_CORE_LIST_H
#include "serializable.h"
#include <iterator>
#include <vector>

template <typename T>
struct List : public Serializable {
private:
    std::vector<T> _data;
public:
    List()
    {
        this->_data = {};
    }
    explicit List(size_t size)
    {
        this->_data = std::vector<T>(size);
    }
    explicit List(const std::vector<T>& data)
    {
        this->_data = data;
    }
    List(const List<T>& other)
    {
        this->_data = other._data;
    }
    List<T>& operator=(const List<T>& other)
    {
        this->_data = other._data;
        return *this;
    }
    T& operator[](size_t index)
    {
        return this->_data[index];
    }
    const T& operator[](size_t index) const
    {
        return this->_data[index];
    }
    size_t size() const
    {
        return this->_data.size();
    }
    void add(const T& value)
    {
        this->_data.push_back(value);
    }
    void remove(size_t index)
    {
        this->_data.erase(this->_data.begin() + index);
    }
    void clear()
    {
        this->_data.clear();
    }
    void resize(size_t size)
    {
        this->_data.resize(size);
    }
    T* data()
    {
        return this->_data.data();
    }
    const T* data() const
    {
        return this->_data.data();
    }
    explicit operator std::vector<T>() const
    {
        return this->_data;
    }
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list)
    {
        os << "[";
        for(size_t i = 0; i < list.size(); i++)
        {
            os << list[i];
            if(i < list.size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
    void serialize(Archive& ar) const override
    {
        ar << this->_data.size();
        for(const T& value : this->_data)
        {
            ar << value;
        }
    }
    void deserialize(Archive& ar) override {
        size_t size;
        ar >> &size;
        this->_data.resize(size);
        for (T &value: this->_data) {
            ar >> &value;
        }
    }
};


#endif //POMEGRANATE_ENGINE_LIST_H
