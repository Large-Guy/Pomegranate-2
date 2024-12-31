#include "alias.h"
#include "input_manager.h"

void ButtonAlias::addButton(Keyboard::Key key) {
    Button button{};
    button.type = Button::KEYBOARD;
    button.button = key;
    buttons.add(button);
}

void ButtonAlias::addButton(Mouse::Button button) {
    Button b{};
    b.type = Button::MOUSE;
    b.button = button;
    buttons.add(b);
}

void ButtonAlias::addButton(Gamepad::Button button) {
    Button b{};
    b.type = Button::GAMEPAD;
    b.gamepad = GAMEPAD_ANY;
    b.button = button;
    buttons.add(b);
}

void ButtonAlias::addButton(GamepadID gamepad, Gamepad::Button button) {
    Button b{};
    b.type = Button::GAMEPAD;
    b.gamepad = gamepad;
    b.button = button;
    buttons.add(b);
}

ButtonState ButtonAlias::getState() {
    ButtonState state = BUTTON_IDLE;

    for(Button option : buttons) {
        switch (option.type) {
            case Button::KEYBOARD: {
                auto key = (Keyboard::Key)option.button;
                ButtonState keyState = _inputManager->getKeyboard().getKey(key);

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
                ButtonState buttonState = _inputManager->getMouse().getButton(button);

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
                ButtonState buttonState = _inputManager->getGamepad(gamepad).getButton(button);

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

void ButtonAlias::onIdle(Function callback) const {
    Event::on(_onIdle, callback);
}

void ButtonAlias::onHeld(Function callback) const {
    Event::on(_onHeld, callback);
}

void ButtonAlias::onPressed(Function callback) const {
    Event::on(_onPressed, callback);
}

void ButtonAlias::onReleased(Function callback) const {
    Event::on(_onReleased, callback);
}

void AxisAlias::addAxis(Axis axis) {
    axes.add(axis);
}

float AxisAlias::getState() {
    auto &options = axes;

    std::vector<float> values{};
    values.reserve(options.size());

    for (auto &option: options) {
        switch (option.type) {
            case Axis::KEYBOARD: {
                auto positive = (Keyboard::Key) option.positive;
                auto negative = (Keyboard::Key) option.negative;

                float positiveValue = _inputManager->getKeyboard().getKey(positive) == BUTTON_HELD ? 1.0f : 0.0f;
                float negativeValue = _inputManager->getKeyboard().getKey(negative) == BUTTON_HELD ? -1.0f : 0.0f;

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
                        positiveValue = _inputManager->getMouse().getDelta().x;
                        break;
                    case Mouse::AXIS_DELTA_Y:
                        positiveValue = _inputManager->getMouse().getDelta().y;
                        break;
                    case Mouse::AXIS_SCROLL_X:
                        positiveValue = _inputManager->getMouse().getScroll().x;
                        break;
                    case Mouse::AXIS_SCROLL_Y:
                        positiveValue = _inputManager->getMouse().getScroll().y;
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

                float value = _inputManager->getGamepad(gamepad).getAxis(axis);

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

                float positiveValue = _inputManager->getGamepad(gamepad).getButton(positive) == BUTTON_HELD ? 1.0f : 0.0f;
                float negativeValue = _inputManager->getGamepad(gamepad).getButton(negative) == BUTTON_HELD ? -1.0f : 0.0f;

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

    if(abs(maxValue) < deadzone)
    {
        return 0.0f;
    }

    if(snap)
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

    if(invert)
    {
        return -maxValue;
    }

    return maxValue;
}

void AxisAlias::onChanged(Function callback) const {
    Event::on(_onChanged, callback);
}

ButtonAlias Alias::button() {
    if(_type != BUTTON) {
        Debug::Log::warn("Alias is not a button alias");
        return {};
    }
    return *(ButtonAlias*)_alias;
}

AxisAlias Alias::axis() {
    if(_type != AXIS) {
        Debug::Log::warn("Alias is not an axis alias");
        return {};
    }
    return *(AxisAlias*)_alias;
}

Alias::Type Alias::type() {
    return _type;
}

bool Alias::exists() {
    return _type != NOT_FOUND;
}

Alias::Alias() {
    _type = NOT_FOUND;
}

Alias::Alias(ButtonAlias& alias) {
    _type = BUTTON;
    _alias = new ButtonAlias(alias);
}

Alias::Alias(AxisAlias& alias) {
    _type = AXIS;
    _alias = new AxisAlias(alias);
}