#include "keyboard.h"

ButtonState Keyboard::getKey(Keyboard::Key key) const {
    return _keys[key];
}