#include "resource.h"

#include <utility>

uint Resource::resourceCount = 0;

Resource::Resource(std::string path, std::string name) {
    this->_id = 0;
    this->_runtime = path.empty();
    this->_name = std::move(name);
    this->_path = std::move(path);
}

void Resource::reload() {

}

std::string Resource::getName() const {
    return this->_name;
}

std::string Resource::getPath() const {
    return this->_path;
}

uint Resource::getId() const {
    return this->_id;
}

void Resource::serialize(Archive &a) const {

}

void Resource::deserialize(Archive &a) {

}
