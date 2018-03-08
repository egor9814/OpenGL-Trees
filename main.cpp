#include <iostream>
#include "component.hpp"
#include "canvas_opengl.hpp"
#include "random.hpp"
#include "search_tree.hpp"
#include "config.hpp"

void fullscreenToggle(Config* config, RectF bounds) {
    config->lastState.fullscreen = !config->lastState.fullscreen;
    if (config->lastState.fullscreen) {
        config->lastState.width = bounds.width();
        config->lastState.height = bounds.height();
        OpenGLApplication::fullscreen();
    } else {
        OpenGLApplication::restoreWindow(0, 0, config->lastState.width, config->lastState.height);
    }
}

class MyTree;
class MainWindow : public Component {
    Config* config;
    BinaryTree* tree;

public:
    explicit MainWindow(MyTree* tree);

    ~MainWindow() override {
        delete config;
    }

    bool onKeyDown(KeyEvent &event) override {
        if(!tree->onKeyDown(event)){
            switch (event.getSpecialKey()){
                default:
                    return Component::onKeyDown(event);
                case KeyEvent::KEY_F11:
                    ::fullscreenToggle(config, getBounds());
                    return true;
            }
        }
        return true;
    }

    bool onKeyUp(KeyEvent &event) override {
        if(!tree->onKeyUp(event))
            return Component::onKeyUp(event);
        return true;;
    }
};


class MyTree : public SearchTree {
    Config* config = nullptr;
public:
    MyTree() : SearchTree() {}
    MyTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator)
            : SearchTree(keyCount, randomKeyGenerator) {}
    MyTree(unsigned long keyCount, int *keys) : SearchTree(keyCount, keys) {}
    explicit MyTree(std::vector<int> &keys) : SearchTree(keys) {}

    void setConfig(Config* config){
        this->config = config;
    }

protected:
    int getColorForPath(unsigned long nodeIndex) override {
        if(config == nullptr)
            return SearchTree::getColorForPath(nodeIndex);
        switch (nodeIndex){
            default:
                return config->binaryTree.pathColor;
            case 0:
                return config->binaryTree.pathColorLeft;
            case 1:
                return config->binaryTree.pathColorRight;
        }
    }
    int getColorForNode(unsigned long nodeIndex) override {
        if(config == nullptr)
            return SearchTree::getColorForNode(nodeIndex);
        switch (nodeIndex){
            default:
                return config->binaryTree.color;
            case 0:
                return config->binaryTree.colorLeft;
            case 1:
                return config->binaryTree.colorRight;
        }
    }

    int getTextColor() override {
        return config == nullptr ? SearchTree::getTextColor() : config->binaryTree.textColor;
    }

    int getBackgroundColor() override {
        return config == nullptr ? SearchTree::getBackgroundColor() : config->binaryTree.backgroundColor;
    }
};

MainWindow::MainWindow(MyTree *tree)
        : tree(tree){
    config = defaultConfig();
    setBackground(tree);
    tree->setConfig(config);
}

/*class TestView : public Component {
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
};*/



int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    Random r;
    auto const count = 15 + r.nextInt(11);

    MyTree tree((unsigned long) count, [&](std::vector<int>& keys) -> int {
        auto k = r.nextInt(1000);
        bool has = true;
        while(has){
            has = false;
            for(auto &i : keys){
                if(i == k){
                    has = true;
                    break;
                }
            }
            if(has){
                k = r.nextInt(1000);
            }
        }
        return k;
    });

    MainWindow window(&tree);

    OpenGLApplication app(&window);
    app.init(argc, argv, true);
    app.setExitCallback([]() {
        std::cout << "finished!" << std::endl;
		Random::free();
    });

    app.start("TestView", 800, 600);

    return 0;
}