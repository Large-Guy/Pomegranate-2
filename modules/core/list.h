#ifndef POMEGRANATE_ENGINE_CORE_LIST_H
#define POMEGRANATE_ENGINE_CORE_LIST_H
#include "serializable.h"
#include <iterator>
#include <vector>

template <typename T>
struct List {
private:
    std::vector<T> _data;
public:
    List()
    {

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
    ~List()
    {
        this->_data.clear();
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
    void insert(size_t index, const T& value)
    {
        this->_data.insert(this->_data.begin() + index, value);
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
    void reserve(size_t size)
    {
        this->_data.reserve(size);
    }
    T& last()
    {
        return this->_data.back();
    }
    T& first()
    {
        return this->_data.front();
    }
    T& at(size_t index)
    {
        return this->_data.at(index);
    }
    T* data()
    {
        return this->_data.data();
    }
    typename std::vector<T>::iterator begin()
    {
        return this->_data.begin();
    }
    typename std::vector<T>::iterator end()
    {
        return this->_data.end();
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
    void serialize(Archive& ar) const
    {
        ar << this->_data.size();
        for(const T& value : this->_data)
        {
            ar << value;
        }
    }
    void deserialize(Archive& ar) {
        size_t size = 0;
        ar >> size;
        this->_data.resize(size);
        for (T &value: this->_data) {
            //ar >> value;
        }
    }
};


#endif //POMEGRANATE_ENGINE_LIST_H
