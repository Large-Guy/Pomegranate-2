#ifndef POMEGRANATEENGINE_ALIASES_H
#define POMEGRANATEENGINE_ALIASES_H

#include <graphics/opengl/window.h>
#include <variant>
#include "enumerations.h"
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"
#include "button.h"
#include "axis.h"

struct ButtonAlias {
    List<Button> buttons;

    void addButton(Keyboard::Key key);
    void addButton(Mouse::Button button);
    void addButton(Gamepad::Button button);
    void addButton(GamepadID gamepad, Gamepad::Button button);
};

struct AxisAlias {
    List<Axis> axes;
    bool invert;
    bool snap;
    float deadzone;

    void addAxis(Axis axis);
};

#endif //POMEGRANATEENGINE_ALIASES_H
