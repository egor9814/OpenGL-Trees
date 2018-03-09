//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_CANVAS_OPENGL_HPP
#define OPENGL_TREES_CANVAS_OPENGL_HPP

#include <functional>
#include <thread>
#include "canvas.hpp"
#include "component.hpp"

class CanvasGL : public Canvas {
    float w = 0, h = 0;
    bool aa = false;

    bool antialiasing(bool enable);

public:
    /** Resize canvas */
    void resize(float width, float height);


    float getWidth() override;

    float getHeight() override;

    void drawText(float x, float y, float z, const char *text, float scale, Paint paint) override;

    void draw(RectF rect, Paint paint) override;

    void draw(Paint paint) override;

    void drawPath(unsigned long pointCount, float *x, float *y, Paint paint) override;

    void drawArc(float x, float y, float radius, float startAngle, float endAngle, Paint paint) override;

    void drawCircle(float x, float y, float radius, Paint paint) override;

    void save() override;

    void restore() override;

    void translate(float x, float y, float z) override;

    void rotate(float angle, float x, float y, float z) override;

    void scale(float x, float y, float z) override;
};


class DrawableCallbackGL : public DrawableCallback {
public:
    void requestInvalidateSelf() override;
};


class OpenGLApplication {
    CanvasGL *canvasGL = nullptr;
    DrawableCallbackGL *drawableCallbackGL = nullptr;
    bool doubleBuffer = false;
    Component *component;

    std::function<void()> exitCallback;

public:
    static void fullscreen();

    static void quit();

    static void restoreWindow(int x, int y, int w, int h);


    explicit OpenGLApplication(Component *);

    ~OpenGLApplication();

    void init(int &argc, char *argv[], bool doubleBuffer);

    void setExitCallback(std::function<void()> &&exitCallback);

    void start(const char *title, int width, int height);

    std::thread *startAsync(const char *title, int width, int height);


    // slots:
    void signal_onDraw();

    void signal_onKeyPress(unsigned char &key, int &x, int &y);

    void signal_onKeyRelease(unsigned char &key, int &x, int &y);

    void signal_onKeySpecialPress(int &key, int &x, int &y);

    void signal_onKeySpecialRelease(int &key, int &x, int &y);

    void signal_onWindowResize(int &w, int &h);

    void signal_onTerminated();
};

#endif //OPENGL_TREES_CANVAS_OPENGL_HPP
