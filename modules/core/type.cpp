#include "type.h"

TypeInfo::TypeInfo() : size(0), type(0), name(nullptr), mangledName(nullptr) {}

TypeInfo::TypeInfo(const TypeInfo& other) {
    size = other.size;
    type = other.type;
    name = other.name;
    mangledName = other.mangledName;
}

bool TypeInfo::operator==(const TypeInfo& other) const {
    return type == other.type;
}