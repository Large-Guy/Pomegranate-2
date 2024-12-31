#ifndef POMEGRANATEENGINE_INPUT_MANAGER_H
#define POMEGRANATEENGINE_INPUT_MANAGER_H
#include "enumerations.h"
#include <variant>
#include <math.h>
#include "keyboard.h"
#include "mouse.h"
#include "gamepad.h"
#include "button.h"
#include "axis.h"
#include "alias.h"
#include <array>

struct Window;

class InputManager {
private:
    Keyboard _keyboard;
    Mouse _mouse;
    std::array<Gamepad,17> _gamepads; //1 extra for AnyGamepad
    Window* attachedWindow;

    std::unordered_map<std::string, float> _axisPreviousValues;

    std::unordered_map<std::string,ButtonAlias> _buttonAliases;
    std::unordered_map<std::string,AxisAlias> _axisAliases;
public:
    InputManager();
    InputManager(Window* window);
    void update();
    void attachWindow(Window* window);
    Window* getAttachedWindow();
    ButtonAlias& addButtonAlias(const std::string& name, ButtonAlias alias);
    AxisAlias& addAxisAlias(const std::string& name, AxisAlias alias);
    void setAxisAliasDeadzone(const std::string& name, float deadzone);

    template<typename...Buttons>
    ButtonAlias& addButtonAlias(const std::string& name, Buttons...buttons) {
        if(_buttonAliases.find(name) != _buttonAliases.end()){
            ButtonAlias& alias = _buttonAliases[name];
            alias._inputManager = this;
            alias._onIdle = Event::create("@"+name+"-idle");
            alias._onHeld = Event::create("@"+name+"-held");
            alias._onPressed = Event::create("@"+name+"-pressed");
            alias._onReleased = Event::create("@"+name+"-released");
            (alias.addButton(buttons), ...);
            return alias;
        }
        ButtonAlias alias{};
        alias._inputManager = this;
        alias._onIdle = Event::create("@"+name+"-idle");
        alias._onHeld = Event::create("@"+name+"-held");
        alias._onPressed = Event::create("@"+name+"-pressed");
        alias._onReleased = Event::create("@"+name+"-released");
        (alias.addButton(buttons), ...);
        _buttonAliases[name] = alias;
        return _buttonAliases[name];
    }

    template<typename...Axes>
    AxisAlias& addAxisAlias(const std::string& name, Axes...axes) {
        if(_axisAliases.find(name) != _axisAliases.end()){
            AxisAlias& alias = _axisAliases[name];
            alias._inputManager = this;
            alias._onChanged = Event::create("@"+name+"-changed");
            (alias.addAxis(axes), ...);
            return alias;
        }
        AxisAlias alias{};
        alias._inputManager = this;
        alias._onChanged = Event::create("@"+name+"-changed");
        (alias.addAxis(axes), ...);
        _axisAliases[name] = alias;
        return _axisAliases[name];
    }


    Keyboard& getKeyboard();
    Mouse& getMouse();
    Gamepad& getGamepad(GamepadID gamepad);

    ButtonAlias& getButtonAlias(const std::string& name);
    AxisAlias& getAxisAlias(const std::string& name);
    Alias getAlias(const std::string& name);
};


#endif //POMEGRANATEENGINE_INPUT_MANAGER_H
