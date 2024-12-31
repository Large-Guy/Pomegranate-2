#include "gamepad.h"
#include "input_manager.h"

GamepadID Gamepad::getID() const {
    return _id;
}

std::string Gamepad::getName() const {
    return _name;
}

bool Gamepad::isConnected() const {
    if(_id == GAMEPAD_ANY) {
        for(int i = 0; i < 16; i++)
        {
            if(_inputManager->getGamepad((GamepadID)i).isConnected())
            {
                return true;
            }
        }
    }

    return _connected;
}

float Gamepad::getAxis(Gamepad::Axis axis) const {
    if(axis == AXIS_NONE)
    {
        return 0.0f;
    }

    if(_id == GAMEPAD_ANY) {
        float greatestValue = 0.0f;
        for(int i = 0; i < 16; i++)
        {
            if(_inputManager->getGamepad((GamepadID)i).isConnected())
            {
                float value = _inputManager->getGamepad((GamepadID)i).getAxis(axis);
                if(abs(value) > abs(greatestValue))
                {
                    greatestValue = value;
                }
            }
        }

        return greatestValue;
    }


    if(axis == AXIS_LEFT_Y || axis == AXIS_RIGHT_Y)
    {
        return -_axes[axis];
    }

    return _axes[axis];
}

ButtonState Gamepad::getButton(Gamepad::Button button) const {
    if(_id == GAMEPAD_ANY) {
        ButtonState greatestState = BUTTON_IDLE;

        for(int i = 0; i < 16; i++)
        {
            if(_inputManager->getGamepad((GamepadID)i).isConnected())
            {
                ButtonState state = _inputManager->getGamepad((GamepadID)i).getButton(button);
                if((int)state > (int)greatestState)
                {
                    greatestState = state;
                }
            }
        }

        return greatestState;
    }

    return _buttons[button];
}