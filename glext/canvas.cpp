//
// Created by egor9814 on 07.03.18.
//

#include "include/GLext/canvas.hpp"

Paint &Paint::setAntiAlias(bool enabled) {
    antiAlias = enabled;
    return *this;
}

bool Paint::isAntiAlias() const {
    return antiAlias;
}

Paint &Paint::setWidth(float w) {
    width = w;
    return *this;
}

float Paint::getWidth() const {
    return width;
}

Paint &Paint::setColor(int c) {
    color = c;
    return *this;
}

int Paint::getColor() const {
    return color;
}


float Drawable::getWidth() const {
    return width;
}

float Drawable::getHeight() const {
    return height;
}

void Drawable::setWidth(float w) {
    auto old = getBounds();
    width = w;
    onBoundsChanged(old, getBounds());
}

void Drawable::setHeight(float h) {
    auto old = getBounds();
    height = h;
    onBoundsChanged(old, getBounds());
}


void Drawable::setX(float x) {
    auto old = getBounds();
    this->x = x;
    onBoundsChanged(old, getBounds());
}

float Drawable::getX() const {
    return x;
}

void Drawable::setY(float y) {
    auto old = getBounds();
    this->y = y;
    onBoundsChanged(old, getBounds());
}

float Drawable::getY() const {
    return y;
}


RectF Drawable::getBounds() {
    return {x, y, x + getWidth(), y + getHeight()};
}

void Drawable::setBounds(RectF bounds) {
    RectF old = getBounds();

    x = bounds.left;
    y = bounds.top;
    width = bounds.width();
    height = bounds.height();

    onBoundsChanged(old, bounds);
}


Drawable *Drawable::getParent() const {
    return parent;
}

void Drawable::setParent(Drawable *drawable) {
    parent = drawable;
}


void Drawable::setDrawableCallback(DrawableCallback *drawableCallback) {
    this->drawableCallback = drawableCallback;
}

void Drawable::invalidateSelf() {
    if(parent)
        parent->invalidateSelf();
    else if (drawableCallback) {
        drawableCallback->requestInvalidateSelf();
    }
}
