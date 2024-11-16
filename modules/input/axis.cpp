#include "axis.h"

Axis::Axis() {
    gamepad = GAMEPAD_ANY;
    gamepadAxis = Gamepad::AXIS_NONE;
    positive = 0;
    negative = 0;
}

Axis::Axis(Keyboard::Key positive, Keyboard::Key negative, bool invert) {
    type = KEYBOARD;
    gamepad = GAMEPAD_ANY;
    gamepadAxis = Gamepad::AXIS_NONE;
    this->positive = positive;
    this->negative = negative;
    this->invert = invert;
}

Axis::Axis(Mouse::Axis positive, Mouse::Axis negative, bool invert) {
    type = MOUSE;
    gamepad = GAMEPAD_ANY;
    gamepadAxis = Gamepad::AXIS_NONE;
    this->positive = positive;
    this->negative = negative;
    this->invert = invert;
}

Axis::Axis(Gamepad::Axis axis, bool invert) {
    type = GAMEPAD_AXIS;
    gamepad = GAMEPAD_ANY;
    gamepadAxis = axis;
    this->invert = invert;
}

Axis::Axis(GamepadID gamepad, Gamepad::Axis axis, bool invert) {
    type = GAMEPAD_AXIS;
    this->gamepad = gamepad;
    gamepadAxis = axis;
    this->invert = invert;
}

Axis::Axis(Gamepad::Button positive, Gamepad::Button negative, bool invert) {
    type = GAMEPAD_BUTTONS;
    gamepad = GAMEPAD_ANY;
    gamepadAxis = Gamepad::AXIS_NONE;
    this->positive = positive;
    this->negative = negative;
    this->invert = invert;
}

Axis::Axis(GamepadID gamepad, Gamepad::Button positive, Gamepad::Button negative, bool invert) {
    type = GAMEPAD_BUTTONS;
    this->gamepad = gamepad;
    gamepadAxis = Gamepad::AXIS_NONE;
    this->positive = positive;
    this->negative = negative;
    this->invert = invert;
}