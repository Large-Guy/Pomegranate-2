#include "asset.h"

#include <utility>

uint Asset::resourceCount = 0;

Asset::Asset(std::string path, std::string name) {
    this->_id = 0;
    this->_runtime = path.empty();
    this->_name = std::move(name);
    this->_path = std::move(path);
}

void Asset::reload() {

}

std::string Asset::getName() const {
    return this->_name;
}

std::string Asset::getPath() const {
    return this->_path;
}

uint Asset::getId() const {
    return this->_id;
}

void Asset::serialize(Archive &a) const {

}

void Asset::deserialize(Archive &a) {

}
