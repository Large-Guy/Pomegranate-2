#include "mesh_instance.h"

MeshInstance::MeshInstance() {
    this->mesh = nullptr;
    property("mesh", &this->mesh);
}

MeshInstance::MeshInstance(MeshBase* mesh) {
    this->mesh = mesh;
    property("mesh", &this->mesh);
}

void MeshInstance::serialize(Archive& a) const {
    //a << this->mesh;
}

void MeshInstance::deserialize(Archive& a) {
    //a >> this->mesh;
}
