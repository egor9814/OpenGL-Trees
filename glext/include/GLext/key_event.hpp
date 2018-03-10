//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_KEY_EVENT_HPP
#define OPENGL_TREES_KEY_EVENT_HPP

/** Key event */
class KeyEvent {
    float x, y;
    char key;
    int specialKey;

public:
    static const int KEY_F1 = 1;
    static const int KEY_F2 = 2;
    static const int KEY_F3 = 3;
    static const int KEY_F4 = 4;
    static const int KEY_F5 = 5;
    static const int KEY_F6 = 6;
    static const int KEY_F7 = 7;
    static const int KEY_F8 = 8;
    static const int KEY_F9 = 9;
    static const int KEY_F10 = 10;
    static const int KEY_F11 = 11;
    static const int KEY_F12 = 12;

    KeyEvent(float x, float y, char key, int specialKey);

    /** Get X key down/up location */
    float getX() const;

    /** Get Y key down/up location */
    float getY() const;

    /** Get key character */
    char getKey() const;

    /** Get special key code */
    int getSpecialKey() const;
};


/** Key event listener */
class KeyEventListener {
public:
    /** Call when key is down
     * @return true if event is prepared, otherwise false
     * */
    virtual bool onKeyDown(KeyEvent &) = 0;

    /** Call when key is up
     * @return true if event is prepared, otherwise false
     * */
    virtual bool onKeyUp(KeyEvent &) = 0;
};


#endif //OPENGL_TREES_KEY_EVENT_HPP
