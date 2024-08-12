#include "c_camera.h"

component_id COMPONENT_CAMERA = 0;

Camera* Camera::_current = nullptr;

void Camera::makeCurrent() {
    if(_current != nullptr)
    {
        _current->_active = false;
    }
    _current = this;
    _active = true;
}

bool Camera::isActive() const {
    return _active;
}