#include "alias.h"

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

void AxisAlias::addAxis(Axis axis) {
    axes.add(axis);
}