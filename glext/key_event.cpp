//
// Created by egor9814 on 07.03.18.
//

#include <GL/glut.h>
#include "include/GLext/key_event.hpp"

KeyEvent::KeyEvent(float x, float y, char key, int specialKey)
        : x(x), y(y), key(key), specialKey(specialKey) {}

float KeyEvent::getX() const {
    return x;
}

float KeyEvent::getY() const {
    return y;
}

char KeyEvent::getKey() const {
    return key;
}

int KeyEvent::getSpecialKey() const {
    return specialKey;
}

