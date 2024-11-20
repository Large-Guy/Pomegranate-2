#include "material.h"

Material::Material() {
    this->shader = nullptr;
    property("shader", &this->shader);
}

Material::Material(ShaderBase* shader) {
    this->shader = shader;
    property("shader", &this->shader);
}

void Material::serialize(Archive& a) const {
    //a << this->shader;
}

void Material::deserialize(Archive& a) {
    //a >> this->shader;
}