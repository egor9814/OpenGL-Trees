//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_DRAWABLES_HPP
#define OPENGL_TREES_DRAWABLES_HPP

#include "canvas.hpp"

class ColorDrawable : public Drawable {
	int color;

public:
	explicit ColorDrawable(int color = 0);

	void setColor(int color);
	int getColor() const ;

	void draw(Canvas *canvas) override;
};

#endif //OPENGL_TREES_DRAWABLES_HPP
