#ifndef POMEGRANATEENGINE_TABLE_H
#define POMEGRANATEENGINE_TABLE_H
#include "serializable.h"
#include <unordered_map>
#include <any>

struct Table : public Serializable {
private:
    struct iKey {

    };
    template<typename K, typename V, typename Hash = std::hash<K>>
    struct Key : public iKey {
        std::unordered_map<K, V, Hash> data;
    };

    std::unordered_map<size_t, iKey*> _data;
public:
    Table();
    Table(const Table& other);
    Table& operator=(const Table& other);

    template<typename K, typename V, typename Hash = std::hash<K>>
    void set(const K& key, const V& value)
    {
        if (this->_data.find(typeid(K).hash_code()) == this->_data.end())
        {
            this->_data[typeid(K).hash_code()] = new Key<K, V, Hash>();
        }
        auto key_data = static_cast<Key<K, V, Hash>*>(this->_data[typeid(K).hash_code()]);
        key_data->data[key] = value;
    }

    template<typename K, typename V, typename Hash = std::hash<K>>
    V& get(const K& key)
    {
        auto key_data = static_cast<Key<K, V, Hash>*>(this->_data[typeid(K).hash_code()]);
        return key_data->data[key];
    }
};


#endif //POMEGRANATEENGINE_TABLE_H
