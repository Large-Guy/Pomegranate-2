#include "transform3d.h"

Transform3D::Transform3D() {
    this->position = Vector3();
    this->scale = Vector3(1, 1, 1);
    this->rotation = 0;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Transform3D::Transform3D(const Vector3& position, const Vector3& scale, const Vector3& rotation) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Transform3D::Transform3D(const Transform3D& other) {
    this->position = other.position;
    this->scale = other.scale;
    this->rotation = other.rotation;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Matrix4x4 Transform3D::getLocalMatrix() const {
    return Matrix4x4::transform(this->position, this->scale, this->rotation);
}

Vector3 Transform3D::getPosition(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    auto* parent = entity.get<Parent>();
    if(parent != nullptr)
    {
        return getPosition(parent->parent) + transform->position;
    }
    return transform->position;
}

Vector3 Transform3D::getScale(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    auto* parent = entity.get<Parent>();
    if(parent != nullptr)
    {
        return getScale(parent->parent) * transform->scale;
    }
    return transform->scale;
}

Vector3 Transform3D::getRotation(const Entity& entity) {
    auto *transform = entity.get<Transform3D>();
    if (transform == nullptr) {
        return {};
    }
    auto *parent = entity.get<Parent>();
    if (parent != nullptr) {
        return getRotation(parent->parent) + transform->rotation;
    }
    return transform->rotation;
}


Matrix4x4 Transform3D::getMatrix(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    Vector3 position = getPosition(entity);
    Vector3 scale = getScale(entity);
    Vector3 rotation = getRotation(entity);
    return Matrix4x4().rotateZ(rotation.z).rotateY(rotation.y).rotateX(rotation.x).scale(scale).translate(position);
}

Matrix4x4 Transform3D::getLocalMatrix(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    return transform->getLocalMatrix();
}

Vector3 Transform3D::getForward(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    Matrix4x4 matrix = getMatrix(entity);
    return matrix.forward();
}

Vector3 Transform3D::getUp(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    Matrix4x4 matrix = getMatrix(entity);
    return matrix.up();
}

Vector3 Transform3D::getRight(const Entity& entity) {
    auto* transform = entity.get<Transform3D>();
    if(transform == nullptr)
    {
        return {};
    }
    Matrix4x4 matrix = getMatrix(entity);
    return matrix.right();
}

void Transform3D::serialize(Archive& a) const {
    a << this->position;
    a << this->scale;
    a << this->rotation;
}

void Transform3D::deserialize(Archive& a) {
    a >> this->position;
    a >> this->scale;
    a >> this->rotation;
}