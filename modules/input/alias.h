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
#include <events/event_manager.h>

struct ButtonAlias {
private:
    EventID _onIdle;
    EventID _onHeld;
    EventID _onPressed;
    EventID _onReleased;
    InputManager* _inputManager;
public:

    List<Button> buttons;

    void addButton(Keyboard::Key key);
    void addButton(Mouse::Button button);
    void addButton(Gamepad::Button button);
    void addButton(GamepadID gamepad, Gamepad::Button button);

    ButtonState getState();

    void onIdle(Function callback) const;
    void onHeld(Function callback) const;
    void onPressed(Function callback) const;
    void onReleased(Function callback) const;

    friend class InputManager;
};

struct AxisAlias {
private:
    EventID _onChanged;
    InputManager* _inputManager;
public:
    List<Axis> axes;
    bool invert;
    bool snap;
    float deadzone;

    void addAxis(Axis axis);

    float getState();

    void onChanged(Function callback) const;

    friend class InputManager;
};



struct Alias
{
public:
    enum Type {
        NOT_FOUND,
        BUTTON,
        AXIS
    };
private:
    void* _alias;
    Type _type;
public:
    ButtonAlias button();
    AxisAlias axis();
    Type type();
    bool exists();

    Alias();
    Alias(ButtonAlias& alias);
    Alias(AxisAlias& alias);
};

#endif //POMEGRANATEENGINE_ALIASES_H
