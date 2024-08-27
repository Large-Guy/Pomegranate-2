#include "transform3d.h"

Transform3D::Transform3D() {
    this->position = Vector3();
    this->scale = Vector3(1, 1, 1);
    this->rotation = 0;
    property<Vector3>("position", &this->position);
    property<Vector3>("scale", &this->scale);
    property<Vector3>("rotation", &this->rotation);
}

Transform3D::Transform3D(const Vector3& position, const Vector3& scale, const Vector3& rotation) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
    property<Vector3>("position", &this->position);
    property<Vector3>("scale", &this->scale);
    property<Vector3>("rotation", &this->rotation);
}

Vector3 Transform3D::getPosition(Entity entity) {
    auto* transform = entity.getComponent<Transform3D>("Transform3D");
    if(transform == nullptr)
    {
        return {};
    }
    auto* parent = entity.getComponent<Parent>("Parent");
    if(parent != nullptr)
    {
        return getPosition(parent->parent) + transform->position;
    }
    return transform->position;
}

Vector3 Transform3D::getScale(Entity entity) {
    auto* transform = entity.getComponent<Transform3D>("Transform3D");
    if(transform == nullptr)
    {
        return {};
    }
    auto* parent = entity.getComponent<Parent>("Parent");
    if(parent != nullptr)
    {
        return getScale(parent->parent) * transform->scale;
    }
    return transform->scale;
}

Vector3 Transform3D::getRotation(Entity entity) {
    auto *transform = entity.getComponent<Transform3D>("Transform3D");
    if (transform == nullptr) {
        return {};
    }
    auto *parent = entity.getComponent<Parent>("Parent");
    if (parent != nullptr) {
        return getRotation(parent->parent) + transform->rotation;
    }
    return transform->rotation;
}


Matrix4x4 Transform3D::getMatrix(Entity entity) {
    auto* transform = entity.getComponent<Transform3D>("Transform3D");
    if(transform == nullptr)
    {
        return {};
    }
    Vector3 position = getPosition(entity);
    Vector3 scale = getScale(entity);
    Vector3 rotation = getRotation(entity);
    return Matrix4x4().rotateZ(rotation.z).rotateY(rotation.y).rotateX(rotation.x).scale(scale).translate(position);
}