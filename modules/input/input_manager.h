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

    std::unordered_map<std::string,ButtonAlias> _buttonAliases;
    std::unordered_map<std::string,AxisAlias> _axisAliases;
public:
    InputManager();
    InputManager(Window* window);
    void update();
    void attachWindow(Window* window);
    Window* getAttachedWindow();
    void addButtonAlias(const std::string& name, ButtonAlias alias);
    void addAxisAlias(const std::string& name, AxisAlias alias);
    void setAxisAliasDeadzone(const std::string& name, float deadzone);

    template<typename...Buttons>
    void addButtonAlias(const std::string& name, Buttons...buttons) {
        if(_buttonAliases.find(name) != _buttonAliases.end()){
            ButtonAlias& alias = _buttonAliases[name];
            (alias.addButton(buttons), ...);
            return;
        }
        ButtonAlias alias{};
        (alias.addButton(buttons), ...);
        _buttonAliases[name] = alias;
    }

    template<typename...Axes>
    void addAxisAlias(const std::string& name, Axes...axes) {
        if(_axisAliases.find(name) != _axisAliases.end()){
            AxisAlias& alias = _axisAliases[name];
            (alias.addAxis(axes), ...);
            return;
        }
        AxisAlias alias{};
        (alias.addAxis(axes), ...);
        _axisAliases[name] = alias;
    }


    Keyboard& getKeyboard();
    Mouse& getMouse();
    Gamepad& getGamepad(GamepadID gamepad);

    ButtonState getButtonAlias(const std::string& name);
    float getAxisAlias(const std::string& name);
};


#endif //POMEGRANATEENGINE_INPUT_MANAGER_H
