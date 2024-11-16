#ifndef POMEGRANATEENGINE_AXIS_H
#define POMEGRANATEENGINE_AXIS_H

#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"

struct Axis {
    enum AxisType {
        KEYBOARD,
        MOUSE,
        GAMEPAD_AXIS,
        GAMEPAD_BUTTONS
    };
    AxisType type;
    int gamepad;
    Gamepad::Axis gamepadAxis;
    int positive;
    int negative;
    bool invert;
    Axis();
    Axis(Keyboard::Key positive, Keyboard::Key negative, bool invert = false);
    Axis(Mouse::Axis positive, Mouse::Axis negative, bool invert = false);
    Axis(Gamepad::Axis positive, bool invert = false);
    Axis(GamepadID gamepad, Gamepad::Axis positive, bool invert = false);
    Axis(Gamepad::Button positive, Gamepad::Button negative, bool invert = false);
    Axis(GamepadID gamepad, Gamepad::Button positive, Gamepad::Button negative, bool invert = false);
};

#endif //POMEGRANATEENGINE_AXIS_H
