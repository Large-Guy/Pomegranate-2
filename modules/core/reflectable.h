#ifndef POMEGRANATEENGINE_REFLECTOR_H
#define POMEGRANATEENGINE_REFLECTOR_H

#include <unordered_map>
#include "serializable.h"

class Reflectable {
private:

    struct Property{
        void* data;
        size_t size;
        size_t type;
    };

    //This needs a custom map implementation so that it can be memcpy'd
    std::unordered_map<std::string, Property> _members;

protected:
    void property(std::string name, void* member, size_t size, size_t type);

    template<typename T> void property(std::string name, T* member) {
        _members[name] = {(void*)member, sizeof(T), typeid(T).hash_code()};
    }

public:
    Reflectable();

    void set(std::string name, void* value);

    template<typename T> void set(std::string name, T value) {
        memcpy(_members[name].data, &value, _members[name].size);
    }

    void* get(std::string name);

    template<typename T> T& get(std::string name) {
        return *(T*)_members[name].data;
    }

    bool has(std::string name);

    size_t type(std::string name);

    size_t getSize(std::string name);

    std::unordered_map<std::string, Property>& getProperties();

    virtual void serialize(Archive& archive) const{};
    virtual void deserialize(Archive& archive){};
};


#endif //POMEGRANATEENGINE_REFLECTOR_H
