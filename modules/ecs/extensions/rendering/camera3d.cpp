#include "camera3d.h"

Entity Camera3D::_mainCamera = nullptr;

Camera3D::Camera3D() {
    property("fov", &this->fov);
    property("near", &this->near);
    property("far", &this->far);
}

Camera3D::Camera3D(float fov, float near, float far) {
    this->fov = fov;
    this->near = near;
    this->far = far;
    property("fov", &this->fov);
    property("near", &this->near);
    property("far", &this->far);
}

Matrix4x4 Camera3D::getProjectionMatrix() const {
    Vector2i size = Window::getCurrent()->getSize();
    float aspect = (float)size.x / (float)size.y;
    return Matrix4x4::perspective(this->fov, aspect, this->near, this->far);
}

void Camera3D::setMain(Entity& entity) {
    _mainCamera = entity;
}

Entity Camera3D::getMain() {
    return _mainCamera;
}

bool Camera3D::isMain() const {
    return _mainCamera.get<Camera3D>() == this;
}

void Camera3D::serialize(Archive& a) const {
    a << this->fov;
    a << this->near;
    a << this->far;
}

void Camera3D::deserialize(Archive& a) {
    a >> this->fov;
    a >> this->near;
    a >> this->far;
}