#include <iostream>
#include "component.hpp"
#include "canvas_opengl.hpp"

struct WindowState {
    int w = 0, h = 0;
    bool fullscreen = false;

    void fullscreenToggle(RectF bounds) {
        fullscreen = !fullscreen;
        if (fullscreen) {
            w = bounds.width();
            h = bounds.height();
            OpenGLApplication::fullscreen();
        } else {
            OpenGLApplication::restoreWindow(0, 0, w, h);
        }
    }
};

static WindowState state;


class TestView : public Component {
    ColorDrawable color;
    RectF padding{50, 50, 50, 50};
    int mode = 0;

public:
    TestView()
            : color(0xff00ff00) {
    }

    void onBoundsChanged(RectF oldBounds, RectF newBounds) override {
        Component::onBoundsChanged(oldBounds, newBounds);
    }

    void draw(Canvas *canvas) override {
        Component::draw(canvas);
        auto rect = getBounds();
        RectF bounds{
                rect.left + padding.left,
                rect.top + padding.top,
                rect.right - padding.right,
                rect.bottom - padding.bottom
        };
        color.setBounds(bounds);
        color.draw(canvas);
    }

    bool onKeyDown(KeyEvent &event) override {
        switch (event.getKey()) {
            case 'l':
                mode = 1;
                return true;
            case 't':
                mode = 2;
                return true;
            case 'r':
                mode = 3;
                return true;
            case 'b':
                mode = 4;
                break;
            case 'c':
                mode = 0;
            case 'p':
            case 'm': {
                int inc = event.getKey() == 'p' ? -1 : 1;
                switch (mode) {
                    default:
                        break;
                    case 0:
                        padding.left += inc;
                        padding.top += inc;
                        padding.right += inc;
                        padding.bottom += inc;
                        break;
                    case 1:
                        padding.left += inc;
                        break;
                    case 2:
                        padding.top += inc;
                        break;
                    case 3:
                        padding.right += inc;
                        break;
                    case 4:
                        padding.bottom += inc;
                        break;
                }
                invalidateSelf();
            }
                return true;

            case 'f':
                ::state.fullscreenToggle(getBounds());
                return true;

            case 'q':
                OpenGLApplication::quit();
                return false;

            default:
                return Component::onKeyDown(event);
        }
    }
};

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    RectF bounds{0, 0, 600, 400};

    TestView test;
    //test.setBounds(bounds);
    test.setBackgroundColor(0xffff0000);

    OpenGLApplication app(&test);
    app.init(argc, argv, true);
    app.setExitCallback([]() {
        std::cout << "finished!" << std::endl;
    });

    app.start("TestView", bounds.width(), bounds.height());

    return 0;
}