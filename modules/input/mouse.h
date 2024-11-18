#ifndef POMEGRANATEENGINE_MOUSE_H
#define POMEGRANATEENGINE_MOUSE_H
#include <math/math.h>
#include "enumerations.h"

class InputManager;

class Mouse {
private:
    Vector2 _position;
    Vector2 _scroll;
    Vector2 _delta;
    ButtonState _buttons[3];
    InputManager* _inputManager;
public:
    enum Button {
        BUTTON_LEFT,
        BUTTON_RIGHT,
        BUTTON_MIDDLE
    };
    enum Axis {
        AXIS_DELTA_X,
        AXIS_DELTA_Y,
        AXIS_SCROLL_X,
        AXIS_SCROLL_Y
    };

    Vector2 getPosition() const;
    Vector2 getScroll() const;
    Vector2 getDelta() const;
    ButtonState getButton(Button button) const;

    void setPosition(Vector2 position);

    friend class InputManager;
};

#endif //POMEGRANATEENGINE_MOUSE_H
