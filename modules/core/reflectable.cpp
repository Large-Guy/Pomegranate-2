#include "reflectable.h"

void Reflectable::property(const char *name, void* member, size_t size, size_t type) {
    _members[name] = Property{member, size, type};
}

void Reflectable::set(const char *name, void *value) {
    size_t size = _members[name].size;
    memcpy(_members[name].data, value, size);
}

void* Reflectable::get(const char *name) {
    return _members[name].data;
}

size_t Reflectable::getSize(const char *name) {
    return _members[name].size;
}

std::unordered_map<const char *, Reflectable::Property, Reflectable::StringHash, Reflectable::StringEqual> &Reflectable::getProperties() {
    return _members;
}