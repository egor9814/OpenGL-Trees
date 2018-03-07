//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_COMPONENT_HPP
#define OPENGL_TREES_COMPONENT_HPP

#include "canvas.hpp"
#include "drawables.hpp"
#include "key_event.hpp"

/** Drawing component with key key prepare support */
class Component : public Drawable, public KeyEventListener {
    Drawable *background = nullptr;
    ColorDrawable *colorDrawable;
    //Component* parent;

public:
    Component();

    virtual ~Component();


    void setParent(Drawable *parent) override;

    void setParent(Component *component);

    Component *getParent() const override;


    /** Set background of component */
    void setBackground(Drawable *drawable);

    /** Set background color of component */
    void setBackgroundColor(int color);


    void draw(Canvas *canvas) override;

    void onBoundsChanged(RectF oldBounds, RectF newBounds) override;

    bool onKeyDown(KeyEvent &event) override;

    bool onKeyUp(KeyEvent &event) override;
};

#endif //OPENGL_TREES_COMPONENT_HPP
