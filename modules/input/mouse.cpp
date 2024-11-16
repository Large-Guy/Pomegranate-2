#include "mouse.h"

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