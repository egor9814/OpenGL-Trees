#include <iostream>
#include <sstream>
#include <GLext/component.hpp>
#include <GLext/canvas_opengl.hpp>
#include <GLext/random.hpp>
#include "search_tree.hpp"
#include "config.hpp"

void fullscreenToggle(Config *config, RectF bounds) {
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
    Config *config;
    BinaryTree *tree;

public:
    explicit MainWindow(MyTree *tree);

    ~MainWindow() override {
        delete config;
    }

    bool onKeyDown(KeyEvent &event) override {
        if (!tree->onKeyDown(event)) {
            switch (event.getSpecialKey()) {
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
        if (!tree->onKeyUp(event))
            return Component::onKeyUp(event);
        return true;;
    }
};


class MyTree : public SearchTree {
    Config *config = nullptr;
public:
    MyTree() : SearchTree() {}

    MyTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator)
            : SearchTree(keyCount, randomKeyGenerator) {}

    MyTree(unsigned long keyCount, int *keys) : SearchTree(keyCount, keys) {}

    explicit MyTree(std::vector<int> &keys) : SearchTree(keys) {}

    void setConfig(Config *config) {
        this->config = config;
    }

protected:
    int getColorForPath(unsigned long nodeIndex) override {
        if (config == nullptr)
            return SearchTree::getColorForPath(nodeIndex);
        switch (nodeIndex) {
            default:
                return config->binaryTree.pathColor;
            case 0:
                return config->binaryTree.pathColorLeft;
            case 1:
                return config->binaryTree.pathColorRight;
        }
    }

    int getColorForNode(unsigned long nodeIndex) override {
        if (config == nullptr)
            return SearchTree::getColorForNode(nodeIndex);
        switch (nodeIndex) {
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
        : tree(tree) {
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


std::vector<std::string> split(std::string &str) {
    using namespace std;
    stringstream ss(str);
    vector<string> parts;
    string part;
    ss >> part;
    while (!ss.eof()) {
        parts.push_back(part);
        ss >> part;
    }
    return parts;
}

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    using namespace std;

    Random r;
    auto const count = 15 + r.nextInt(11);

    MyTree tree((unsigned long) count, [&](std::vector<int> &keys) -> int {
        auto k = r.nextInt(1000);
        bool has = true;
        while (has) {
            has = false;
            for (auto &i : keys) {
                if (i == k) {
                    has = true;
                    break;
                }
            }
            if (has) {
                k = r.nextInt(1000);
            }
        }
        return k;
    });

    MainWindow window(&tree);

    OpenGLApplication app(&window);
    app.init(argc, argv, true);

    auto thread = app.startAsync("TestView", 800, 600);

    app.setExitCallback([&]() {
        std::cout << "finished!" << std::endl;
        Random::free();
        //delete thread;
    });

    cout << "Welcome to Trees REPL" << endl;
    cout << "Type 'help' for get help" << endl;
    while (true) {
        cout << "cmd> ";
        std::string cmd;
        //cin >> cmd;
        getline(cin, cmd);
        cmd += " ";

        auto command = split(cmd);
        if (command.empty()) {
            cerr << "error: please type something" << endl;
            continue;
        }
        if (command[0] == "exit") {
            cout << "Goodbye!" << endl;
            OpenGLApplication::quit();
            break;
        } else if (command[0] == "help") {
            if (command.size() > 1) {
                if (command[1] == "help") {
                    cout << "get help" << endl;
                } else if (command[1] == "exit") {
                    cout << "finish app" << endl;
                } else if (command[1] == "find") {
                    cout << "find <key> - find and show node by key" << endl;
                } else if (command[1] == "add") {
                    cout << "add <key> - add key to tree" << endl;
                } else if (command[1] == "remove") {
                    cout << "remove <key> - remove key from tree"
                            "(if <key> == all, then tree will be removed)" << endl;
                } else if (command[1] == "get") {
                    cout << "get <property> - get property of tree:" << endl;
                    cout << "\tproperties:" << endl;
                    cout << "\t\theight - get tree's height" << endl;
                    //cout << "\t\t" << endl;
                } else if (command[1] == "visit") {
                    cout << "visit <type> - visit tree by type(by default: lrR)" << endl;
                    cout << "\ttypes:" << endl;
                    cout << "\t\tRlr - Root left right" << endl;
                    cout << "\t\tRrl - Root right left" << endl;
                    cout << "\t\tlRr - left Root right" << endl;
                    cout << "\t\trRl - right Root left" << endl;
                    cout << "\t\tlvl - by levels" << endl;
                    cout << "\t\tlrR - left right Root" << endl;
                } else {
                    cerr << "error: unknown command: " << command[1] << endl;
                }
            } else {
                cout << "commands: " << endl << "\thelp, exit, find, add, remove, get, visit" << endl;
                cout << "type 'help <command>' for get more info about command" << endl;
            }
        } else if (command[0] == "find") {
            if (command.size() < 2) {
                cerr << "type 'help find'" << endl;
            } else {
                auto key = stoi(command[1]);
                tree.findKeyAndDraw(key);
            }
        } else if (command[0] == "add") {
            if (command.size() < 2) {
                cerr << "type 'help add'" << endl;
            } else {
                auto key = stoi(command[1]);
                tree.add(key);
                tree.requestRedraw();
            }
        } else if (command[0] == "remove") {
            if (command.size() < 2) {
                cerr << "type 'help remove'" << endl;
            } else {
                if (command[1] == "all") {
                    tree.clear();
                } else {
                    auto key = stoi(command[1]);
                    tree.remove(key);
                }
                tree.requestRedraw();
            }
        } else if (command[0] == "visit") {
            std::string type = command.size() < 2 ? "lrR" : command[1];
            auto visit = [](MyTree &tree_, MyTree::VisitType type_) {
                auto res = tree_.visit(type_);
                for (auto &i : res) {
                    cout << i << "  ";
                }
                cout << endl;
            };
            if (type == "Rlr") {
                cout << "root left right: " << endl;
                visit(tree, MyTree::VisitType_RootLeftRight);
            } else if (type == "Rrl") {
                cout << "root right left: " << endl;
                visit(tree, MyTree::VisitType_RootRightLeft);
            } else if (type == "lRr") {
                cout << "left root right: " << endl;
                visit(tree, MyTree::VisitType_LeftRootRight);
            } else if (type == "rRl") {
                cout << "right root left: " << endl;
                visit(tree, MyTree::VisitType_RightRootLeft);
            } else if (type == "lvl") {
                cout << "by levels: " << endl;
                visit(tree, MyTree::VisitType_ByLevels);
            } else if (type == "lrR") {
                cout << "left right root: " << endl;
                visit(tree, MyTree::VisitType_LeftRightRoot);
            } else {
                cerr << "error: unknown type: " << type << endl << "type 'help visit'" << endl;
            }
        } else {
            cerr << "error: unknown command: " << command[0] << endl;
        }
    }

    return 0;
}