#ifndef POMEGRANATEENGINE_BUTTON_H
#define POMEGRANATEENGINE_BUTTON_H

#include "gamepad.h"

struct Button {
    enum ButtonType {
        KEYBOARD,
        MOUSE,
        GAMEPAD
    };
    ButtonType type;
    GamepadID gamepad;
    int button;
};

#endif //POMEGRANATEENGINE_BUTTON_H
