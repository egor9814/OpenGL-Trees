//
// Created by egor9814 on 07.03.18.
//

#include "include/GLext/drawables.hpp"

ColorDrawable::ColorDrawable(int color)
        : color(color) {}

void ColorDrawable::setColor(int color) {
    this->color = color;
}

int ColorDrawable::getColor() const {
    return color;
}


void ColorDrawable::draw(Canvas *canvas) {
    if (canvas)
        canvas->draw(getBounds(), Paint().setColor(color));
}
