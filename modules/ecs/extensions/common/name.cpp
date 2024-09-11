#include "name.h"

Name::Name() {
    this->name = "";
    property("_name", &this->name);
}

Name::Name(const String& name) {
    this->name = name;
    property("_name", &this->name);
}

void Name::serialize(Archive& a) const {
    a << this->name;
}

void Name::deserialize(Archive& a) {
    a >> this->name;
}