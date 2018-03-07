//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_CANVAS_HPP
#define OPENGL_TREES_CANVAS_HPP

#include <vector>

/** Rectangle */
template <typename PointType = int>
struct Rect {
	PointType left, top, right, bottom;

	PointType width() const {
		return right - left;
	}

	PointType height() const {
		return bottom - top;
	}


	PointType centerX() const {
		return left + width()/2;
	}

	PointType centerY() const {
		return top + height()/2;
	}

	Rect& translateX(PointType delta){
		left += delta;
		right += delta;
		return *this;
	}

	Rect& translateY(PointType delta){
		top += delta;
		bottom += delta;
		return *this;
	}

	Rect& translate(PointType deltaX, PointType deltaY){
		left += deltaX;
		top += deltaY;
		right += deltaX;
		bottom += deltaY;
		return *this;
	}
};
typedef Rect<float> RectF;


/** Point */
template <typename PointType = int>
struct Point {
	PointType x, y;
};
typedef Point<float> PointF;


class Canvas;

/** Paint params */
class Paint {
	friend Canvas;

	bool antiAlias;
	int color;
	float width;

public:
	explicit Paint(bool antiAlias = false)
			: antiAlias(antiAlias), color(0), width(1){}

	Paint& setAntiAlias(bool enabled);
	bool isAntiAlias() const ;

	Paint& setWidth(float w);
	float getWidth() const ;

	Paint& setColor(int c);
	int getColor() const ;
};


/** Canvas */
class Canvas {
public:
	/** Get canvas width */
	virtual float getWidth() = 0;
	/** Get canvas height */
	virtual float getHeight() = 0;

	/** Draw text on canvas
	 * @param x: X-axis translation
	 * @param y: Y-axis translation
	 * @param z: Z-axis translation
	 * @param text: your text
	 * @param scale: font scale
	 * @param paint: font style
	 * */
	virtual void drawText(float x, float y, float z, const char* text, float scale, Paint paint) = 0;

	/** Draw rectangle on canvas
	 * @param rect: drawing area
	 * @param paint: style
	 * */
	virtual void draw(RectF rect, Paint paint) = 0;

	/** Fill canvas
	 * @param paint: style
	 * */
	virtual void draw(Paint paint) = 0;

	/** Draw path-line on canvas
	 * @param pointCount: number of path points
	 * @param x: array of X points
	 * @param y: array of Y points
	 * @param paint: style
	 * */
	virtual void drawPath(unsigned long pointCount, float x[], float y[], Paint paint) = 0;

	/** Draw arc line on canvas
	 * @param x: X-axis translation
	 * @param y: Y-axis translation
	 * @param radius: radius of arc
	 * @param startAngle: start arc's angle in Degrees
	 * @param endAngle: end arc's angle in Degrees
	 * @param paint: style
	 * */
	virtual void drawArc(float x, float y, float radius, float startAngle, float endAngle, Paint paint) = 0;

	/** Draw circle on canvas
	 * @param x: X-axis translation
	 * @param y: Y-axis translation
	 * @param radius: radius of circle
	 * @param paint: style
	 * */
	virtual void drawCircle(float x, float y, float radius, Paint paint) = 0;

	/** Save drawing matrix */
	virtual void save() = 0;
	/** Restore drawing matrix */
	virtual void restore() = 0;

	/** Translate canvas
	 * @param x: X-axis translation
	 * @param y: Y-axis translation
	 * @param z: Z-axis translation
	 * */
	virtual void translate(float x, float y, float z = 0) = 0;

	/** Rotate canvas
	 * @param angle: angle in Degrees
	 * @param x: X-axis rotation
	 * @param y: Y-axis rotation
	 * @param z: Z-axis rotation
	 * */
	virtual void rotate(float angle, float x, float y, float z = 0) = 0;

	/** Scale canvas
	 * @param x: X-axis scaling
	 * @param y: Y-axis scaling
	 * @param z: Z-axis scaling
	 * */
	virtual void scale(float x, float y, float z = 0) = 0;
};


/** Callback class for Drawable's calls */
class DrawableCallback {
public:
	virtual void requestInvalidateSelf() = 0;
};

/** Class for drawing on canvas */
class Drawable {
	float x=0, y=0;
	float width=0, height=0;
	Drawable* parent = nullptr;
	DrawableCallback* drawableCallback = nullptr;

protected:
	/** Request redrawing */
	void invalidateSelf();

public:
	/** Draw method */
	virtual void draw(Canvas* canvas) = 0;

	void setX(float x);
	float getX() const ;
	void setY(float y);
	float getY() const ;

	float getWidth() const ;
	float getHeight() const ;
	void setWidth(float w);
	void setHeight(float h);

	/** Get drawing bounds */
	RectF getBounds();
	/** Set drawing bounds */
	void setBounds(RectF bounds);

	/** Callback method, when bounds has been changed */
	virtual void onBoundsChanged(RectF oldBounds, RectF newBounds){}

	/** Get parent */
	virtual Drawable* getParent() const ;
	/** Set parent */
	virtual void setParent(Drawable* drawable);

	/** Set callbacks */
	virtual void setDrawableCallback(DrawableCallback* drawableCallback);
};

#endif //OPENGL_TREES_CANVAS_HPP
