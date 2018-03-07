//
// Created by egor9814 on 07.03.18.
//

#include "component.hpp"

Component::Component() {
	colorDrawable = new ColorDrawable();
}

Component::~Component() {
	delete colorDrawable;
}


void Component::setParent(Drawable *parent) {
	throw "unsupported operation: Component::setParent(Drawable*)";
}

void Component::setParent(Component *component) {
	Drawable::setParent(component);
}

Component* Component::getParent() const {
	return dynamic_cast<Component*>(Drawable::getParent());
}


void Component::setBackground(Drawable *drawable) {
	if(background)
		background->setParent(nullptr);
	background = drawable;
	if(background) {
		background->setBounds(getBounds());
		background->setParent(this);
	}
	invalidateSelf();
}

void Component::setBackgroundColor(int color) {
	colorDrawable->setColor(color);
	setBackground(colorDrawable);
}


void Component::draw(Canvas *canvas) {
	if(background)
		background->draw(canvas);
}


bool Component::onKeyDown(KeyEvent &event) {
	return false;
}

bool Component::onKeyUp(KeyEvent &event) {
	return false;
}

void Component::onBoundsChanged(RectF oldBounds, RectF newBounds) {
	Drawable::onBoundsChanged(oldBounds, newBounds);
	if(background)
		background->setBounds(newBounds);
}

