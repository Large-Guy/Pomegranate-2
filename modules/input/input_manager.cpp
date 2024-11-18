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
}

void InputManager::attachWindow(Window* window) {
    attachedWindow = window;
}

Window* InputManager::getAttachedWindow() {
    return attachedWindow;
}

void InputManager::addButtonAlias(const std::string& name, ButtonAlias alias) {
    _buttonAliases[name] = std::move(alias);
}

void InputManager::addAxisAlias(const std::string& name, AxisAlias alias) {
    _axisAliases[name] = std::move(alias);
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

ButtonState InputManager::getButtonAlias(const std::string& name) {
    if(_buttonAliases.find(name) == _buttonAliases.end()) {
        Debug::Log::warn("Button Alias",name,"not found!");
        return BUTTON_IDLE;
    }
    ButtonAlias alias = _buttonAliases[name];
    auto& options = alias.buttons;

    ButtonState state = BUTTON_IDLE;

    for(auto & option : options) {
        switch (option.type) {
            case Button::KEYBOARD: {
                auto key = (Keyboard::Key)option.button;
                ButtonState keyState = getKeyboard().getKey(key);

                //Prioritize just pressed and just released
                if(keyState == BUTTON_PRESSED) {
                    return BUTTON_PRESSED;
                }
                if(keyState == BUTTON_RELEASED) {
                    state = BUTTON_RELEASED;
                }
                if(keyState == BUTTON_HELD) {
                    state = BUTTON_HELD;
                }
                break;
            }
            case Button::MOUSE: {
                auto button = (Mouse::Button)option.button;
                ButtonState buttonState = getMouse().getButton(button);

                //Prioritize just pressed and just released
                if(buttonState == BUTTON_PRESSED) {
                    return BUTTON_PRESSED;
                }
                if(buttonState == BUTTON_RELEASED) {
                    state = BUTTON_RELEASED;
                }
                if(buttonState == BUTTON_HELD) {
                    state = BUTTON_HELD;
                }
                break;
            }
            case Button::GAMEPAD: {
                auto gamepad = (GamepadID)option.gamepad;
                auto button = (Gamepad::Button)option.button;
                ButtonState buttonState = getGamepad(gamepad).getButton(button);

                //Prioritize just pressed and just released
                if(buttonState == BUTTON_PRESSED) {
                    return BUTTON_PRESSED;
                }
                if(buttonState == BUTTON_RELEASED) {
                    state = BUTTON_RELEASED;
                }
                if(buttonState == BUTTON_HELD) {
                    state = BUTTON_HELD;
                }
                break;
            }
        }
    }

    return state;
}

float InputManager::getAxisAlias(const std::string& name) {
    if (_axisAliases.find(name) == _axisAliases.end()) {
        Debug::Log::warn("Axis Alias", name, "not found!");
        return 0.0f;
    }
    AxisAlias alias = _axisAliases[name];
    auto &options = alias.axes;

    std::vector<float> values{};
    values.reserve(options.size());

    for (auto &option: options) {
        switch (option.type) {
            case Axis::KEYBOARD: {
                auto positive = (Keyboard::Key) option.positive;
                auto negative = (Keyboard::Key) option.negative;

                float positiveValue = getKeyboard().getKey(positive) == BUTTON_HELD ? 1.0f : 0.0f;
                float negativeValue = getKeyboard().getKey(negative) == BUTTON_HELD ? -1.0f : 0.0f;

                float value = positiveValue + negativeValue;

                if(option.invert)
                {
                    value = -value;
                }

                values.push_back(value * option.sensitivity);
                break;
            }
            case Axis::MOUSE: {
                auto axis = (Mouse::Axis) option.mouseAxis;

                float positiveValue = 0.0f;

                switch (axis) {
                    case Mouse::AXIS_DELTA_X:
                        positiveValue = getMouse().getDelta().x;
                        break;
                    case Mouse::AXIS_DELTA_Y:
                        positiveValue = getMouse().getDelta().y;
                        break;
                    case Mouse::AXIS_SCROLL_X:
                        positiveValue = getMouse().getScroll().x;
                        break;
                    case Mouse::AXIS_SCROLL_Y:
                        positiveValue = getMouse().getScroll().y;
                        break;
                }

                float value = positiveValue;

                if(option.invert)
                {
                    value = -value;
                }

                values.push_back(value * option.sensitivity);
                break;
            }
            case Axis::GAMEPAD_AXIS: {
                auto gamepad = (GamepadID) option.gamepad;
                auto axis = (Gamepad::Axis) option.gamepadAxis;

                float value = getGamepad(gamepad).getAxis(axis);

                if(option.invert)
                {
                    value = -value;
                }

                values.push_back(value * option.sensitivity);
                break;
            }
            case Axis::GAMEPAD_BUTTONS: {
                auto gamepad = (GamepadID) option.gamepad;
                auto positive = (Gamepad::Button) option.positive;
                auto negative = (Gamepad::Button) option.negative;

                float positiveValue = getGamepad(gamepad).getButton(positive) == BUTTON_HELD ? 1.0f : 0.0f;
                float negativeValue = getGamepad(gamepad).getButton(negative) == BUTTON_HELD ? -1.0f : 0.0f;

                float value = positiveValue + negativeValue;

                if(option.invert)
                {
                    value = -value;
                }

                values.push_back(value * option.sensitivity);
                break;
            }
        }
    }

    float maxValue = 0.0f;
    for(auto & value : values) {
        if(abs(value) > abs(maxValue)) {
            maxValue = value;
        }
    }

    if(abs(maxValue) < alias.deadzone)
    {
        return 0.0f;
    }

    if(alias.snap)
    {
        if(maxValue > 0.0f)
        {
            return 1.0f;
        }
        if(maxValue < 0.0f)
        {
            return -1.0f;
        }
    }

    if(alias.invert)
    {
        return -maxValue;
    }

    return maxValue;
}