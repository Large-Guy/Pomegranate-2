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

    struct StringHash {
        size_t operator()(const char* str) const {
            size_t hash = 5381;
            int c;
            while ((c = *str++)) {
                hash = ((hash << 5) + hash) + c;
            }
            return hash;
        }
    };

    struct StringEqual {
        bool operator()(const char* a, const char* b) const {
            return strcmp(a, b) == 0;
        }
    };

    std::unordered_map<const char*, Property, StringHash, StringEqual> _members{};

protected:
    void property(const char* name, void* member, size_t size, size_t type);

    template<typename T> void property(const char* name, T* member) {
        _members[name] = {(void*)member, sizeof(T), typeid(T).hash_code()};
    }

public:
    void set(const char* name, void* value);

    template<typename T> void set(const char* name, T value) {
        memcpy(_members[name].data, &value, _members[name].size);
    }

    void* get(const char* name);

    template<typename T> T get(const char* name) {
        return *(T*)_members[name].data;
    }

    bool has(const char* name);

    size_t type(const char* name);

    size_t getSize(const char* name);

    std::unordered_map<const char*, Property, StringHash, StringEqual>& getProperties();

    virtual void serialize(Archive& archive) const{};
    virtual void deserialize(Archive& archive){};
};


#endif //POMEGRANATEENGINE_REFLECTOR_H
