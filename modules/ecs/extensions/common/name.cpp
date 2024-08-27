#include "name.h"

Name::Name() {
    this->name = "";
    property<std::string>("name", &this->name);
}

Name::Name(const std::string &name) {
    this->name = name;
    property<std::string>("name", &this->name);
}
