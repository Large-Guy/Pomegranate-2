#include "name.h"

Name::Name() {
    property("name", &this->name);
}

Name::Name(const std::string& name) {
    this->name = name;
    property("name", &this->name);
}

Name::Name(const Name& other) {
    this->name = other.name;
    property("name", &this->name);
}

void Name::serialize(Archive& a) const {
    a << this->name;
}

void Name::deserialize(Archive& a) {
    a >> this->name;
}