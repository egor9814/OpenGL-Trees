//
// Created by egor9814 on 07.03.18.
//

#ifndef OPENGL_TREES_TREE_HPP
#define OPENGL_TREES_TREE_HPP

#include <functional>
#include <GLext/component.hpp>


/** Tree node
 * T - type of key
 * ChildCount - max count of children
 * Node - inheritor class name
 * */
template<typename T, unsigned long ChildCount, class Node>
struct TreeNode {
    typedef T KeyType;

    KeyType key; /// your key
    const unsigned long childCount = ChildCount; /// maximum child count
    Node *parent; /// parent of current node
    Node *child[ChildCount]{nullptr}; /// children of current node
    unsigned long level; /// height of current node
    unsigned long childNum = 0; /// child number (KOSTYL for add new node in simple tree)

    /** Get count of children */
    auto count() const {
        auto count = 0UL;
        for (auto i = 0UL; i < childCount; i++) {
            if (child[i] != nullptr)
                count++;
        }
        return count;
    }

    bool isLeast() const {
        return count() == 0;
    }

    TreeNode(KeyType key, Node *parent)
            : key(key), parent(parent) {
        level = parent == nullptr ? 1UL : (1 + parent->level);
    }


    void setChild(Node* node, unsigned long index) {
        child[index] = node;
        if(node)
            node->parent = dynamic_cast<Node*>(this);
    }


    virtual void draw(Canvas *canvas, float x, float y, float radius) {}

};


/** Tree
 * NodeType - inheritor of TreeNode
 * */
template<class NodeType>
class Tree : public Component {
public:
    typedef NodeType Node;
    typedef typename Node::KeyType KeyType;

protected:
    Node *root = nullptr;

private:
    struct GraphicNode;
    /// Variables for evaluating graphic node's location
    float radius = 40;
    float offset = 50;
    float lineWidth = 2;
    float textSize = 0.12f;

private:

    /** Recursive method for delete all nodes */
    void removeNodes(Node *&root) {
        if (!root)
            return;
        for (unsigned long i = 0; i < root->childCount; i++) {
            if (root->child[i]) {
                removeNodes(root->child[i]);
            }
        }
        delete root;
        root = nullptr;
    }

    /** Add new key */
    void addKey(Node *root, KeyType &key) {
        if (root->count() == root->childCount) {
            addKey(root->child[root->childNum], key);
            root->childNum = (root->childNum + 1) % root->childCount;
        } else {
            //for(auto i = 0UL; i < root->childCount; i++){}
            root->child[root->count()] = new Node(key, root);
        }
    }

    /** Find node by key */
    Node *findNode(KeyType &key, Node *root) {
        if (root == nullptr)
            return nullptr;
        if (root->key == key)
            return root;
        for (unsigned long i = 0; i < root->childCount; i++) {
            auto node = findNode(key, root->child[i]);
            if (node)
                return node;
        }
        return nullptr;
    }

    /** Height of node */
    void height(Node *root, unsigned long &h) {
        if (!root)
            return;
        if (root->level > h)
            h = root->level;
        for (auto i = 0UL; i < root->childCount; i++) {
            height(root->child[i], h);
        }
    }

    unsigned long heightOf(Node* node) {
        if(!node)
            return 0;
        auto h = heightOf(node->child[0]);
        for(auto i = 0ul; i < node->childCount; i++){
            auto hMax = heightOf(node->child[i]);
            if(hMax > h)
                h = hMax;
        }
        return h + 1;
    }

public:
    Tree() = default;

    /** Generate tree with random keys */
    Tree(unsigned long keyCount, std::function<KeyType(std::vector<KeyType> &)> randomKeyGenerator) {
        std::vector<KeyType> keys;
        for (unsigned long i = 0; i < keyCount; i++) {
            keys.push_back(randomKeyGenerator(keys));
        }
        for (auto &i : keys) {
            add(i);
        }
    }

    /** Generate tree with key array */
    Tree(unsigned long keyCount, KeyType *keys) {
        for (unsigned long i = 0; i < keyCount; i++) {
            add(keys[i]);
        }
    }

    /** Generate tree with keys */
    explicit Tree(std::vector<KeyType> &keys) {
        for (auto &i : keys) {
            add(i);
        }
    }

    ~Tree() override {
        clear();
        delete removeCandidate;
        delete foundCandidate;
    }


    /** Clear tree */
    void clear() {
        removeNodes(root);
    }

    /** Get root of tree */
    Node *getRoot() const {
        return root;
    }

    /** Set root of tree */
    void setRoot(Node *node) {
        root = node;
    }


    /** Add new key to tree */
    virtual void add(KeyType key) {
        if (root == nullptr) {
            root = new Node(key, nullptr);
        } else {
            addKey(root, key);
        }
    }

    /** Remove key from tree */
    virtual void remove(KeyType key) {
        throw "cannot remove key, because it is unsupported";
    }

    /** Find node by key */
    virtual Node *findKey(KeyType key) {
        return findNode(key, root);
    }

    /** Find height of tree */
    unsigned long findHeight() {
        /*auto h = 0UL;
        height(root, h);
        return h;*/
        return heightOf(root);
    }


    bool onKeyDown(KeyEvent &event) override {
        switch (event.getKey()) {
            default:
                return false;
            case 'Q':
                exit(0);
                return true;
            case 'a':
                add(randomKey());
                if (foundCandidate) {
                    delete foundCandidate;
                    foundCandidate = nullptr;
                }
                invalidateSelf();
                return true;
            case 'r':
                if (removeCandidate != nullptr) {
                    delete removeCandidate;
                }
                removeCandidate = new RemoveCandidate;
                removeCandidate->x = event.getX();
                removeCandidate->y = event.getY();
                invalidateSelf();
                return true;
            case 'R':
                clear();
                if (foundCandidate) {
                    delete foundCandidate;
                    foundCandidate = nullptr;
                }
                invalidateSelf();
                return true;
            case '+':
                setRadius(getRadius() + 1);
                invalidateSelf();
                return true;
            case '-':
                setRadius(getRadius() - 1);
                invalidateSelf();
                return true;
        }
    }

    bool onKeyUp(KeyEvent &event) override {
        return false;
    }


    void draw(Canvas *canvas) override {
        canvas->draw(Paint().setColor(getBackgroundColor()));
        auto treeH = findHeight();
        if (treeH != 0) {
            //const auto WIDTH = height(root) * root->childCount * R + (root->childCount - 1) * OFFSET;
            //const auto x = (WIDTH + START_OFFSET + OFFSET) / root->childCount;
            auto bounds = getBounds();
            const auto WIDTH = bounds.width() - offset;
            const auto HEIGHT = bounds.height() - offset;

            auto layout = RectF{0, 0, WIDTH, HEIGHT};
            //layout.translate(OFFSET / 2, OFFSET / 2);

            auto offset = HEIGHT / treeH;
            offset = std::max(radius * 1, offset);
            RectF padding{0, this->offset / 2, this->offset / 2, offset};

            std::vector<GraphicNode> nodes;
            getNodes(root, layout, radius, lineWidth, padding, nodes, static_cast<unsigned long>(-1));

            if (removeCandidate && removeCandidate->setup) {
                remove(removeCandidate->key);
                delete removeCandidate;
                removeCandidate = nullptr;
                //invalidateSelf(); // it is very HARD call
                draw(canvas);
            } else {
                if (foundCandidate && foundCandidate->setup) {
                    foundCandidate->draw(canvas, 3 * radius / 4, 0xffffff00);
                    /*delete foundCandidate;
                    foundCandidate = nullptr;*/
                }

                GraphicNode root = nodes[0];
                drawPaths(canvas, root, root.nodes);
                drawCircles(canvas, root, root.nodes, treeH - 1);
            }
        }
    }

    void onBoundsChanged(RectF oldBounds, RectF newBounds) override {
        Component::onBoundsChanged(oldBounds, newBounds);
        if (foundCandidate) {
            delete foundCandidate;
            foundCandidate = nullptr;
        }
    }

protected:
    /** Generate random key */
    virtual KeyType randomKey() {
        throw "cannot make a random key, because it is unsupported";
    }

    /** Get color for node's links path */
    virtual int getColorForPath(unsigned long nodeIndex) {
        return 0xffffffff;
    }

    /** Get color for node's circle */
    virtual int getColorForNode(unsigned long nodeIndex) {
        return 0xffffffff;
    }

    /** Get text colot */
    virtual int getTextColor() {
        return 0xffff0000;
    }

    /** Get background color */
    virtual int getBackgroundColor() {
        return 0xff000000;
    }

private:
    /** Drawing element for node */
    struct GraphicNode {
        std::string key; /// string of key
        unsigned long index; /// index of node
        /** Location type */
        enum Type {
            Top, Center, Bottom
        };
        Type type; /// location of node
        float radius; /// radius of node
        float width; /// line width
        float x, y; /// node's location
        std::vector<GraphicNode> nodes; /// node's children
        NodeType* node;

        GraphicNode(std::string &&key, unsigned long index,
                    Type type,
                    float radius, float width, float x, float y,
                    NodeType* node)
                : key(std::move(key)), index(index),
                  type(type),
                  radius(radius), width(width), x(x), y(y), nodes(),
                  node(node) {}

        GraphicNode(const GraphicNode &n)
                : key(n.key), index(n.index),
                  type(n.type),
                  radius(n.radius), width(n.width), x(n.x), y(n.y), nodes(n.nodes),
                  node(n.node) {}
    };

    /** Generate nodes for draw */
    void getNodes(Node *n, RectF &layout, float &r, float &w, RectF &padding, std::vector<GraphicNode> &nodes,
                  unsigned long index, unsigned long level = 0) {
        if (n == nullptr)
            return;

        //const auto pw = padding.width();
        const auto startX = layout.centerX() + padding.width();
        /*if(startX > layout.right)
            startX = layout.right;
        else if(startX < layout.left)
            startX = layout.left;*/
        const auto startY = padding.top + padding.bottom * level + r / 2;

        if (foundCandidate && !foundCandidate->setup) {
            if (!removeCandidate || !removeCandidate->setup) {
                if (n->key == foundCandidate->key) {
                    foundCandidate->x = startX;
                    foundCandidate->y = startY;
                    foundCandidate->setup = true;
                }
            }
        }

        if (removeCandidate && !removeCandidate->setup) {
            auto x = removeCandidate->x;
            auto y = removeCandidate->y;

            auto hr = r / 2;
            if (x >= startX - hr && x <= startX + hr && y >= startY - hr && y <= startY + hr) {
                removeCandidate->key = n->key;
                removeCandidate->setup = true;
                if (foundCandidate) {
                    delete foundCandidate;
                    foundCandidate = nullptr;
                }
            }/* else {
				delete removeCandidate;
				removeCandidate = nullptr;
			}*/
        }


        auto t = level == 0 ? GraphicNode::Top : (n->count() == 0 ? GraphicNode::Bottom : GraphicNode::Center);
        GraphicNode node(std::to_string(n->key), index, t, r, w, startX, startY, n);

        unsigned long count = n->childCount;
        if (count > 0) {
            const auto width = layout.width() / count;

            /*auto sector = RectF {layout.left, startY, layout.left + width, startY + padding.height() * 2}
                    .translateY(r / 2);*/
            //auto sector = RectF {0, layout.top, width, layout.bottom};
            RectF sector(layout);
            sector.right = layout.left + width;
            /*auto p = layout.left+pw/2;
            if(sector.right + p < layout.right){
                sector.translateX(p);
            }*/

            for (auto i = 0UL; i < count; i++) {
                /*float x[]{ startX, sector.centerX() };
                float y[]{ startY, sector.bottom-r/2 };*/

                getNodes(n->child[i], sector, r, w, padding, node.nodes, i, level + 1);
                sector.translateX(width);
            }
        }

        nodes.push_back(node);
    }

    /** Draw node's link paths */
    void drawPaths(Canvas *canvas, GraphicNode &root, std::vector<GraphicNode> &nodes) {
        auto p = Paint(true).setWidth(root.width);
        for (GraphicNode &i : nodes) {
            float x[]{root.x, i.x, i.x};
            float y[]{root.y + root.radius / 2, root.y + (i.y - root.y) / 2, i.y - root.radius / 2};
            p.setColor(getColorForPath(i.index));
            canvas->drawPath(3, x, y, p);
            drawPaths(canvas, i, i.nodes);
        }
    }

    /** Draw node's circles */
    void drawCircles(Canvas *canvas, GraphicNode &root, std::vector<GraphicNode> &nodes, unsigned long level) {
        auto x = root.x, y = root.y, r = root.radius;
        auto p = Paint(true)
                .setWidth(root.width)
                .setColor(getColorForNode(root.index));
        canvas->drawCircle(x, y, r / 2, p);
        if (root.type == GraphicNode::Top) {
            canvas->drawArc(x, y, 2 * r / 3, -135, 135, p);
        } else if (root.type == GraphicNode::Bottom) {
            canvas->drawArc(x, y, 2 * r / 3, 45, 315, p);
        } else {
            canvas->drawArc(x, y, 2 * r / 3, -135, -45, p);
            canvas->drawArc(x, y, 2 * r / 3, 45, 135, p);
        }
        canvas->drawText(x - r / 2 + 1, y + r / 5, 0, root.key.c_str(), textSize,
                         Paint(true).setColor(getTextColor()));
        for (GraphicNode &i : nodes) {
            drawCircles(canvas, i, i.nodes, level);
        }

        if(root.node)
            root.node->draw(canvas, x, y, r);
    }

    /** Structure for holding candidate node for removing */
    struct RemoveCandidate {
        KeyType key; /// key
        float x, y; /// remove location
        bool setup; /// indicator of node founded
    };
    RemoveCandidate *removeCandidate = nullptr;

    /** Structure for holding candidate node for search location */
    struct FoundCandidate {
        KeyType key; /// key
        float x, y; /// founded locations
        bool setup; /// indicator of node founded

        FoundCandidate(KeyType key, bool setup)
                : key(key), x(0), y(0), setup(setup) {}

        /** Draw founded location */
        inline void draw(Canvas *c, float radius, int color) {
            if (setup) {
                c->draw({x - radius, y - radius, x + radius, y + radius}, Paint().setColor(color));
            }
        }
    };

    FoundCandidate *foundCandidate = nullptr;

public:
    /** Find node by key and draw location */
    void findKeyAndDraw(KeyType key) {
        if (foundCandidate) {
            delete foundCandidate;
        }
        foundCandidate = new FoundCandidate{key, false};
        invalidateSelf();
    }

    void requestRedraw() {
        invalidateSelf();
    }

    float getRadius() const {
        return radius;
    }

    void setRadius(float radius) {
        Tree::radius = radius;
        Tree::textSize = 0.002999999999f * radius;
    }

    float getOffset() const {
        return offset;
    }

    void setOffset(float offset) {
        Tree::offset = offset;
    }

    float getLineWidth() const {
        return lineWidth;
    }

    void setLineWidth(float lineWidth) {
        Tree::lineWidth = lineWidth;
    }
};


#endif //OPENGL_TREES_TREE_HPP
