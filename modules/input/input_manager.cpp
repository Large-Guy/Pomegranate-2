#include "input_manager.h"

#include <utility>

static Vector2 scroll;

InputManager::InputManager() {
    attachedWindow = nullptr;
    _keyboard = {};
    _keyboard._inputManager = this;
    _mouse = {};
    _mouse._inputManager = this;
    for(int i = 0; i < 17; i++) {
        _gamepads[i] = {};
        _gamepads[i]._id = (GamepadID)i;
        _gamepads[i]._inputManager = this;
        _gamepads[i]._connected = false;
    }
}

InputManager::InputManager(Window* window) {
    attachedWindow = window;
    window->_inputManager = this;
    _keyboard = {};
    _keyboard._inputManager = this;
    _mouse = {};
    _mouse._inputManager = this;
    for(int i = 0; i < 17; i++) {
        _gamepads[i] = {};
        _gamepads[i]._id = (GamepadID)i;
        _gamepads[i]._inputManager = this;
        _gamepads[i]._connected = false;
    }
}

void scrollCallback(GLFWwindow* window, double x, double y) {
    scroll = {(float)x, (float)y};
}

void InputManager::update() {
    if(attachedWindow == nullptr) {
        return;
    }
    GLFWwindow* window = attachedWindow->_window;

    glfwSetScrollCallback(window,scrollCallback);

    // Keyboard
    for(int i = 0; i < GLFW_KEY_LAST + 1; i++) {
        if(glfwGetKey(window, i) == GLFW_PRESS)
        {
            switch (_keyboard._keys[i]) {
                case BUTTON_IDLE:
                    _keyboard._keys[i] = BUTTON_PRESSED;
                    break;
                case BUTTON_PRESSED:
                    _keyboard._keys[i] = BUTTON_HELD;
                    break;
            }
        }
        else
        {
            switch (_keyboard._keys[i]) {
                case BUTTON_HELD:
                    _keyboard._keys[i] = BUTTON_RELEASED;
                    break;
                case BUTTON_RELEASED:
                    _keyboard._keys[i] = BUTTON_IDLE;
                    break;
            }
        }
    }

    // Mouse
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    _mouse._delta = {(float)x - _mouse._position.x, (float)y - _mouse._position.y};
    _mouse._position = {(float)x, (float)y};
    _mouse._scroll = scroll;

    for(int i = 0; i < 3; i++) {
        if(glfwGetMouseButton(window, i) == GLFW_PRESS)
        {
            switch (_mouse._buttons[i]) {
                case BUTTON_IDLE:
                    _mouse._buttons[i] = BUTTON_PRESSED;
                    break;
                case BUTTON_PRESSED:
                    _mouse._buttons[i] = BUTTON_HELD;
                    break;
            }
        }
        else
        {
            switch (_mouse._buttons[i]) {
                case BUTTON_HELD:
                    _mouse._buttons[i] = BUTTON_RELEASED;
                    break;
                case BUTTON_RELEASED:
                    _mouse._buttons[i] = BUTTON_IDLE;
                    break;
            }
        }
    }

    // Gamepads
    for(int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i)) {
            _gamepads[i]._connected = true;
            //_gamepads[i]._name = glfwGetGamepadName(i); //This segfaults for some reason

            int count;
            const float* axes = glfwGetJoystickAxes(i, &count);

            for(int j = 0; j < count; j++) {
                _gamepads[i]._axes[j] = axes[j];
            }

            const unsigned char* buttons = glfwGetJoystickButtons(i, &count);

            for(int j = 0; j < count; j++) {
                if(buttons[j] == GLFW_PRESS)
                {
                    switch (_gamepads[i]._buttons[j]) {
                        case BUTTON_IDLE:
                            _gamepads[i]._buttons[j] = BUTTON_PRESSED;
                            break;
                        case BUTTON_PRESSED:
                            _gamepads[i]._buttons[j] = BUTTON_HELD;
                            break;
                    }
                }
                else
                {
                    switch (_gamepads[i]._buttons[j]) {
                        case BUTTON_HELD:
                            _gamepads[i]._buttons[j] = BUTTON_RELEASED;
                            break;
                        case BUTTON_RELEASED:
                            _gamepads[i]._buttons[j] = BUTTON_IDLE;
                            break;
                    }
                }
            }
        }
    }

    //Iterate over all button aliases
    for(auto & [name, alias] : _buttonAliases) {
        ButtonState state = getButtonAlias(name).getState();
        switch (state) {
            case BUTTON_IDLE:
                Event::emit(alias._onIdle);
                break;
            case BUTTON_HELD:
                Event::emit(alias._onHeld);
                break;
            case BUTTON_PRESSED:
                Event::emit(alias._onPressed);
                break;
            case BUTTON_RELEASED:
                Event::emit(alias._onReleased);
                break;
        }
    }

    //Iterate over all axis aliases
    for(auto & [name, alias] : _axisAliases) {
        float value = getAxisAlias(name).getState();
        if(value != _axisPreviousValues[name]) {
            Event::emit(alias._onChanged, value);
        }
        _axisPreviousValues[name] = value;
    }
}

void InputManager::attachWindow(Window* window) {
    attachedWindow = window;
}

Window* InputManager::getAttachedWindow() {
    return attachedWindow;
}

ButtonAlias& InputManager::addButtonAlias(const std::string& name, ButtonAlias alias) {
    _buttonAliases[name] = std::move(alias);
    ButtonAlias& buttonAlias = _buttonAliases[name];
    buttonAlias._inputManager = this;
    buttonAlias._onIdle = Event::create("@"+name+"-idle");
    buttonAlias._onHeld = Event::create("@"+name+"-held");
    buttonAlias._onPressed = Event::create("@"+name+"-pressed");
    buttonAlias._onReleased = Event::create("@"+name+"-released");

    return buttonAlias;
}

AxisAlias& InputManager::addAxisAlias(const std::string& name, AxisAlias alias) {
    _axisAliases[name] = std::move(alias);

    AxisAlias& axisAlias = _axisAliases[name];
    axisAlias._inputManager = this;
    axisAlias._onChanged = Event::create("@"+name+"-changed");


    return axisAlias;
}

void InputManager::setAxisAliasDeadzone(const std::string& name, float deadzone) {
    if(_axisAliases.find(name) == _axisAliases.end()) {
        Debug::Log::warn("Axis Alias",name,"not found!");
        return;
    }
    _axisAliases[name].deadzone = deadzone;
}

Keyboard& InputManager::getKeyboard() {
    return _keyboard;
}

Mouse& InputManager::getMouse() {
    return _mouse;
}

Gamepad& InputManager::getGamepad(GamepadID gamepad) {
    return _gamepads[gamepad];
}

ButtonAlias& InputManager::getButtonAlias(const std::string& name) {
    if(_buttonAliases.find(name) == _buttonAliases.end()) {
        Debug::Log::warn("Button Alias",name,"not found!");
    }
    ButtonAlias& alias = _buttonAliases[name];
    return alias;
}

AxisAlias& InputManager::getAxisAlias(const std::string& name) {
    if (_axisAliases.find(name) == _axisAliases.end()) {
        Debug::Log::warn("Axis Alias", name, "not found!");
    }
    AxisAlias& alias = _axisAliases[name];
    return alias;
}

Alias InputManager::getAlias(const std::string& name) {
    if(_buttonAliases.find(name) != _buttonAliases.end()) {
        return _buttonAliases[name];
    }
    if(_axisAliases.find(name) != _axisAliases.end()) {
        return _axisAliases[name];
    }
    Debug::Log::warn("Alias",name,"not found!");
    return {};
}