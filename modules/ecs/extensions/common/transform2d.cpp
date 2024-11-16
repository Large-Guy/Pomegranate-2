#include "transform2d.h"

Transform2D::Transform2D() {
    this->_initalized = false;
    this->position = Vector2();
    this->scale = Vector2(1, 1);
    this->rotation = 0;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Transform2D::Transform2D(const Vector2& position, const Vector2& scale, float rotation) {
    this->_initalized = false;
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Transform2D::Transform2D(const Transform2D& other) {
    this->_initalized = false;
    this->position = other.position;
    this->scale = other.scale;
    this->rotation = other.rotation;
    property("_position", &this->position);
    property("scale", &this->scale);
    property("rotation", &this->rotation);
}

Vector2 Transform2D::getPosition(Entity& entity) {
    auto* transform = entity.get<Transform2D>();
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

Vector2 Transform2D::getScale(Entity& entity) {
    auto* transform = entity.get<Transform2D>();
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

float Transform2D::getRotation(Entity& entity) {
    auto *transform = entity.get<Transform2D>();
    if (transform == nullptr) {
        return {};
    }
    auto *parent = entity.get<Parent>();
    if (parent != nullptr) {
        return getRotation(parent->parent) + transform->rotation;
    }
    return transform->rotation;
}


Matrix3x3 Transform2D::getMatrix(Entity& entity) {
    auto* transform = entity.get<Transform2D>();
    if(transform == nullptr)
    {
        return {};
    }
    Vector2 position = getPosition(entity);
    Vector2 scale = getScale(entity);
    float rotation = getRotation(entity);
    return Matrix3x3().rotate(rotation).scale(scale).translate(position);
}

void Transform2D::serialize(Archive& a) const {
    a << this->position;
    a << this->scale;
    a << this->rotation;
}

void Transform2D::deserialize(Archive& a) {
    a >> this->position;
    a >> this->scale;
    a >> this->rotation;
}