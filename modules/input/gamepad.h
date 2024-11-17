#ifndef POMEGRANATEENGINE_GAMEPAD_H
#define POMEGRANATEENGINE_GAMEPAD_H

#include "enumerations.h"

class InputManager;

struct Gamepad {
private:
    bool _connected;
    GamepadID _id;
    std::string _name;
    float _axes[6];
    ButtonState _buttons[15];
    InputManager* _inputManager;
public:
    enum Axis {
        AXIS_LEFT_X,
        AXIS_LEFT_Y,
        AXIS_LEFT_TRIGGER,
        AXIS_RIGHT_X,
        AXIS_RIGHT_Y,
        AXIS_RIGHT_TRIGGER,
        AXIS_NONE
    };

    enum Button {
        BUTTON_BOTTOM,
        BUTTON_RIGHT,
        BUTTON_LEFT,
        BUTTON_TOP,
        BUTTON_LEFT_BUMPER,
        BUTTON_RIGHT_BUMPER,
        BUTTON_BACK,
        BUTTON_START,
        BUTTON_GUIDE,
        BUTTON_LEFT_THUMB,
        BUTTON_RIGHT_THUMB,
        BUTTON_DPAD_UP,
        BUTTON_DPAD_RIGHT,
        BUTTON_DPAD_DOWN,
        BUTTON_DPAD_LEFT
    };

    GamepadID getID() const;
    std::string getName() const;
    bool isConnected() const;
    float getAxis(Axis axis) const;
    ButtonState getButton(Button button) const;

    friend class InputManager;
};


#endif //POMEGRANATEENGINE_GAMEPAD_H
