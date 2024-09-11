#include "name.h"

Name::Name() {
    this->name = "";
    property<String>("_name", &this->name);
}

Name::Name(const String& name) {
    this->name = name;
    property<String>("_name", &this->name);
}

void Name::serialize(Archive& a) const {
    a << this->name;
}

void Name::deserialize(Archive& a) {
    a >> this->name;
}