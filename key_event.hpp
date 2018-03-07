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
	KeyEvent(float x, float y, char key, int specialKey);

	/** Get X key down/up location */
	float getX() const ;
	/** Get Y key down/up location */
	float getY() const ;

	/** Get key character */
	char getKey() const ;

	/** Get special key code */
	int getSpecialKey() const ;
};


/** Key event listener */
class KeyEventListener {
public:
	/** Call when key is down
	 * @return true if event is prepared, otherwise false
	 * */
	virtual bool onKeyDown(KeyEvent&) = 0;

	/** Call when key is up
	 * @return true if event is prepared, otherwise false
	 * */
	virtual bool onKeyUp(KeyEvent&) = 0;
};


#endif //OPENGL_TREES_KEY_EVENT_HPP
