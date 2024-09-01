#include "name.h"

Name::Name() {
    this->name = "";
    property<String>("name", &this->name);
}

Name::Name(const String& name) {
    this->name = name;
    property<String>("name", &this->name);
}
