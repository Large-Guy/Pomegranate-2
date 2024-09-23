#include "reflectable.h"

Reflectable::Reflectable() {
    _members = std::unordered_map<std::string, Property>();
}

void Reflectable::property(std::string name, void* member, size_t size, size_t type) {
    _members[name] = {member, size, type};
}

void Reflectable::set(std::string name, void *value) {
    memcpy(_members[name].data, value, _members[name].size);
}

void* Reflectable::get(std::string name) {
    return _members[name].data;
}

bool Reflectable::has(std::string name) {
    bool has = _members.find(name) != _members.end();
    return has;
}

size_t Reflectable::type(std::string name) {
    return _members[name].type;
}

size_t Reflectable::getSize(std::string name) {
    return _members[name].size;
}

std::unordered_map<std::string , Reflectable::Property> &Reflectable::getProperties() {
    return _members;
}