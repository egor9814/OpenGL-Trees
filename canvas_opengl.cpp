//
// Created by egor9814 on 07.03.18.
//

#include "canvas_opengl.hpp"
#include <GL/glut.h>
#include <cmath>

void CanvasGL::resize(float width, float height) {
    w = width;
    h = height;
}

float CanvasGL::getWidth() {
    return w;
}

float CanvasGL::getHeight() {
    return h;
}

bool CanvasGL::antialiasing(bool enable) {
    bool res = aa;
    if (enable != aa) {
        aa = enable;
        if (enable) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        } else {
            glDisable(GL_BLEND);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
        }
    }
    return res;
}


// help functions
void setColor(int color) {
    float a = (color >> 24) & 0xff;
    float r = (color >> 16) & 0xff;
    float g = (color >> 8) & 0xff;
    float b = color & 0xff;
    glColor4f(r / 255, g / 255, b / 255, a / 255);
}

void invert(RectF &r, float h) {
    r.top = h - r.top;
    r.bottom = h - r.bottom;
}

void invert(unsigned long count, float *points, float &h) {
    for (auto i = 0UL; i < count; i++) {
        points[i] = h - points[i];
    }
}

double toRadians(double angle) {
    return angle * M_PI / 180.;
}


// impl

void CanvasGL::drawText(float x, float y, float z, const char *text, float scale, Paint paint) {
    auto aa = antialiasing(paint.isAntiAlias());
    glLineWidth(paint.getWidth());
    ::setColor(paint.getColor());
    y = h - y;

    save();
    translate(x, y, z);
    this->scale(scale, scale, scale);
    for (auto c = text; *c; c++) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
    }
    restore();

    antialiasing(aa);
}

void CanvasGL::draw(RectF rect, Paint paint) {
    bool aa = antialiasing(paint.isAntiAlias());

    glBegin(GL_QUADS);
    ::setColor(paint.getColor());
    glLineWidth(paint.getWidth());
    ::invert(rect, h);
    glVertex2f(rect.left, rect.top);
    glVertex2f(rect.right, rect.top);
    glVertex2f(rect.right, rect.bottom);
    glVertex2f(rect.left, rect.bottom);
    glEnd();

    antialiasing(aa);
}

void CanvasGL::draw(Paint paint) {
    draw({0, 0, w, h}, paint);
}

void CanvasGL::drawPath(unsigned long pointCount, float *x, float *y, Paint paint) {
    auto aa = antialiasing(paint.isAntiAlias());
    glLineWidth(paint.getWidth());
    ::invert(pointCount, y, h);
    glBegin(GL_LINES);
    ::setColor(paint.getColor());
    for (auto i = 0UL; i < pointCount - 1; i++) {
        glVertex2f(x[i], y[i]);
        glVertex2f(x[i + 1], y[i + 1]);
    }
    glEnd();
    antialiasing(aa);
}

void CanvasGL::drawArc(float cx, float cy, float radius, float startAngle, float endAngle, Paint paint) {
    auto aa = antialiasing(paint.isAntiAlias());
    glLineWidth(paint.getWidth());
    ::setColor(paint.getColor());
    cy = h - cy;
    startAngle += 90;
    endAngle += 90;

    glBegin(GL_LINE_STRIP);
    for (auto i = 0; i <= 720; i++) {
        auto angle = ::toRadians(startAngle + (endAngle - startAngle) * i / 720);

        auto x = radius * -std::cos(angle);
        auto y = radius * std::sin(angle);

        glVertex2d(cx + x, cy + y);
    }
    glEnd();

    antialiasing(aa);
}

void CanvasGL::drawCircle(float x, float y, float radius, Paint paint) {
    drawArc(x, y, radius, 0, 360, paint);
}

void CanvasGL::save() {
    glPushMatrix();
}

void CanvasGL::restore() {
    glPopMatrix();
}

void CanvasGL::translate(float x, float y, float z) {
    glTranslatef(x, y, z);
}

void CanvasGL::rotate(float angle, float x, float y, float z) {
    glRotatef(angle, x, y, z);
}

void CanvasGL::scale(float x, float y, float z) {
    glScalef(x, y, z);
}


void DrawableCallbackGL::requestInvalidateSelf() {
    glutPostRedisplay();
}


void OpenGLApplication::fullscreen() {
    glutFullScreen();
}

void OpenGLApplication::quit() {
    exit(0);
}

void OpenGLApplication::restoreWindow(int x, int y, int w, int h) {
    glutReshapeWindow(w, h);
    glutPositionWindow(x, y);
}


static OpenGLApplication *application = nullptr;

OpenGLApplication::OpenGLApplication(Component *c)
        : component(c) {
    if (application != nullptr)
        throw "OpenGL application is launched";
    application = this;
}

OpenGLApplication::~OpenGLApplication() {
    delete canvasGL;
    delete drawableCallbackGL;
    application = nullptr;
}


void onDraw();
void onKeyPress(unsigned char key, int x, int y);
void onKeyRelease(unsigned char key, int x, int y);
void onKeySpecialPress(int key, int x, int y);
void onKeySpecialRelease(int key, int x, int y);
void onWindowResize(int w, int h);
void onTerminated();

void OpenGLApplication::init(int &argc, char **argv, bool doubleBuffer) {
    this->doubleBuffer = doubleBuffer;

    glutInit(&argc, argv);
    unsigned int mode = GLUT_RGBA;
    if (doubleBuffer)
        mode |= GLUT_DOUBLE;
    glutInitDisplayMode(mode);

    atexit(onTerminated);
}

void OpenGLApplication::setExitCallback(std::function<void()> &&exitCallback) {
    this->exitCallback = exitCallback;
}

void OpenGLApplication::start(const char *title, int width, int height) {
    canvasGL = new CanvasGL;
    drawableCallbackGL = new DrawableCallbackGL;
    if (component)
        component->setDrawableCallback(drawableCallbackGL);

    glutInitWindowSize(width, height);
    glutCreateWindow(title);

    glutDisplayFunc(onDraw);
    glutKeyboardFunc(onKeyPress);
    glutKeyboardUpFunc(onKeyRelease);
    glutSpecialFunc(onKeySpecialPress);
    glutSpecialUpFunc(onKeySpecialRelease);
    glutReshapeFunc(onWindowResize);

    glutMainLoop();
}


void onDraw() {
    if (application)
        application->signal_onDraw();
}

void onKeyPress(unsigned char key, int x, int y) {
    if (application)
        application->signal_onKeyPress(key, x, y);
}

void onKeyRelease(unsigned char key, int x, int y) {
    if (application)
        application->signal_onKeyRelease(key, x, y);
}

void onKeySpecialPress(int key, int x, int y) {
    if (application)
        application->signal_onKeySpecialPress(key, x, y);
}

void onKeySpecialRelease(int key, int x, int y) {
    if (application)
        application->signal_onKeySpecialRelease(key, x, y);
}

void onWindowResize(int w, int h) {
    if (application)
        application->signal_onWindowResize(w, h);
}

void onTerminated() {
    if (application)
        application->signal_onTerminated();
}


void OpenGLApplication::signal_onDraw() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (component && canvasGL)
        component->draw(canvasGL);
    glFlush();
    if (doubleBuffer)
        glutSwapBuffers();
}

void OpenGLApplication::signal_onKeyPress(unsigned char &key, int &x, int &y) {
    if (component) {
        KeyEvent event(x, y, key, 0);
        component->onKeyDown(event);
    }
}

void OpenGLApplication::signal_onKeyRelease(unsigned char &key, int &x, int &y) {
    if (component) {
        KeyEvent event(x, y, key, 0);
        component->onKeyUp(event);
    }
}

void OpenGLApplication::signal_onKeySpecialPress(int &key, int &x, int &y) {
    if (component) {
        KeyEvent event(x, y, 0, key);
        component->onKeyDown(event);
    }
}

void OpenGLApplication::signal_onKeySpecialRelease(int &key, int &x, int &y) {
    if (component) {
        KeyEvent event(x, y, 0, key);
        component->onKeyUp(event);
    }
}

void OpenGLApplication::signal_onWindowResize(int &w, int &h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (component) {
        canvasGL->resize(w, h);
        component->setBounds({0, 0, w, h});
    }
}

void OpenGLApplication::signal_onTerminated() {
    exitCallback();
}
