#include "type.h"

TypeInfo::TypeInfo() : size(0), type(0) {
    this->name = "";
    this->mangledName = "";
}

TypeInfo::TypeInfo(const TypeInfo& other) {
    size = other.size;
    type = other.type;
    name = other.name;
    mangledName = other.mangledName;
}

bool TypeInfo::compare(const TypeInfo& other) const {
    return type == other.type;
}

bool TypeInfo::safeCompare(const TypeInfo &other) const {
    return size == other.size && mangledName == other.mangledName;
}