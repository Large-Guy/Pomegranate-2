#ifndef POMEGRANATEENGINE_HASH_TABLE_H
#define POMEGRANATEENGINE_HASH_TABLE_H
#include "serializable.h"
#include <functional>
#include <unordered_map>

template <typename K, typename V, typename Hash = std::hash<K>>
struct HashTable : public Serializable {
private:
    std::unordered_map<K, V, Hash> _data;
public:
    HashTable()
    {
        this->_data = {};
    }
    explicit HashTable(const std::unordered_map<K, V, Hash>& data)
    {
        this->_data = data;
    }
    HashTable(const HashTable<K, V, Hash>& other)
    {
        this->_data = other._data;
    }
    HashTable<K, V, Hash>& operator=(const HashTable<K, V, Hash>& other)
    {
        this->_data = other._data;
        return *this;
    }
    V& operator[](const K& key)
    {
        return this->_data[key];
    }
    const V& operator[](const K& key) const
    {
        return this->_data[key];
    }
    size_t size() const
    {
        return this->_data.size();
    }
    void add(const K& key, const V& value)
    {
        this->_data[key] = value;
    }
    void remove(const K& key)
    {
        this->_data.erase(key);
    }
    void clear()
    {
        this->_data.clear();
    }
    bool has(const K& key) const
    {
        return this->_data.find(key) != this->_data.end();
    }
    size_t count(const K& key) const
    {
        return this->_data.count(key);
    }
    std::unordered_map<K, V, Hash> data() const
    {
        return this->_data;
    }
    void data(const std::unordered_map<K, V, Hash>& data)
    {
        this->_data = data;
    }
    typename std::unordered_map<K, V, Hash>::iterator begin()
    {
        return this->_data.begin();
    }
    typename std::unordered_map<K, V, Hash>::iterator end()
    {
        return this->_data.end();
    }
    typename std::unordered_map<K, V, Hash>::const_iterator begin() const
    {
        return this->_data.begin();
    }
    typename std::unordered_map<K, V, Hash>::const_iterator end() const
    {
        return this->_data.end();
    }

    explicit operator std::unordered_map<K, V, Hash>() const
    {
        return this->_data;
    }
    friend std::ostream& operator<<(std::ostream& os, const HashTable<K,V,Hash>& list)
    {
        os << "[";
        for (auto it = list.begin(); it != list.end(); it++) {
            os << it->first << ": " << it->second;
            if (std::next(it) != list.end()) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
    void serialize(Archive& a) const override
    {
        a << this->_data.size();
        for (auto it = this->_data.begin(); it != this->_data.end(); it++) {
            a << it->first;
            a << it->second;
        }
    }
    void deserialize(Archive& a) override {
        size_t size;
        a >> &size;
        for (size_t i = 0; i < size; i++) {
            K key;
            V value;
            a >> &key;
            a >> &value;
            this->_data[key] = value;
        }
    }
};
#endif //POMEGRANATEENGINE_HASH_TABLE_H
