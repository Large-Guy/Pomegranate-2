#include "mouse.h"
#include "input_manager.h"

Vector2 Mouse::getPosition() const {
    return _position;
}

Vector2 Mouse::getScroll() const {
    return _scroll;
}

Vector2 Mouse::getDelta() const {
    return _delta;
}

ButtonState Mouse::getButton(Mouse::Button button) const {
    return _buttons[button];
}

void Mouse::setPosition(Vector2 position) {
    _position = position;
    glfwSetCursorPos(_inputManager->getAttachedWindow()->_window, position.x, position.y);
}