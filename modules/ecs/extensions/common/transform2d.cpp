#include "transform2d.h"

Transform2D::Transform2D() {
    this->_initalized = false;
    this->position = Vector2();
    this->scale = Vector2(1, 1);
    this->rotation = 0;
    property<Vector2>("position", &this->position);
    property<Vector2>("scale", &this->scale);
    property<float>("rotation", &this->rotation);
}

Transform2D::Transform2D(const Vector2& position, const Vector2& scale, float rotation) {
    this->_initalized = false;
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
    property<Vector2>("position", &this->position);
    property<Vector2>("scale", &this->scale);
    property<float>("rotation", &this->rotation);
}

Matrix3x3 Transform2D::matrix() {
    if(!this->_initalized || this->position != this->_position || this->scale != this->_scale || this->rotation != this->_rotation)
    {
        this->_initalized = true;
        this->_position = this->position;
        this->_scale = this->scale;
        this->_rotation = this->rotation;
        this->_matrix = Matrix3x3().scale(this->scale).rotate(this->rotation).translate(this->position);
    }
    return this->_matrix;
}
