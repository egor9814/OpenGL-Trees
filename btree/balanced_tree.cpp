//
// Created by egor9814 on 10.03.18.
//

#include "balanced_tree.hpp"


int height(BTreeNode* node) {
    return node ? node->h : 0;
}

int balance(BTreeNode* node) {
    return height(node->rightB()) - height(node->leftB());
}

void fixheight(BTreeNode* node) {
    auto l = height(node->leftB());
    auto r = height(node->rightB());
    node->h = std::max(l, r) + 1;
}


void BTreeNode::draw(Canvas *canvas, float x, float y, float radius) {
    canvas->drawText(x + radius/2 + 10, y - radius/2, 0, std::to_string(::balance(this)).c_str(),
                     0.1f, Paint(true).setColor(0xffff0000));
}



BTree::BTree() : SearchTree() {}

BTree::BTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator)
        : SearchTree() {
    std::vector<int> keys;
    for (unsigned long i = 0; i < keyCount; i++) {
        keys.push_back(randomKeyGenerator(keys));
    }
    for (auto &i : keys) {
        add(i);
    }
}

BTree::BTree(unsigned long keyCount, int *keys) : SearchTree() {
    for (unsigned long i = 0; i < keyCount; i++) {
        add(keys[i]);
    }
}

BTree::BTree(std::vector<int> &keys) : SearchTree() {
    for (auto &i : keys) {
        add(i);
    }
}



BTreeNode* rotateRight(BTreeNode* node) {
    auto q = node->leftB();
    node->setLeft(q->rightB());
    q->setRight(node);
    fixheight(node);
    fixheight(q);
    return q;
}

BTreeNode* rotateLeft(BTreeNode* node) {
    auto q = node->rightB();
    node->setRight(q->leftB());
    q->setLeft(node);
    fixheight(node);
    fixheight(q);
    return q;
}

BTreeNode* rebalance(BTreeNode* node) {
    fixheight(node);
    switch (balance(node)){
        case 2:
            if(balance(node->rightB()) < 0) {
                node->setRight(rotateRight(node->rightB()));
            }
            return rotateLeft(node);

        case -2:
            if(balance(node->leftB()) > 0) {
                node->setLeft(rotateLeft(node->leftB()));
            }
            return rotateRight(node);

        default:
            return node;
    }
}

BTreeNode *addNode(BTreeNode *node, int key, BTreeNode *parent = nullptr) {
    if(!node)
        return new BTreeNode(key, parent);

    if(key < node->key)
        node->setLeft(addNode(node->leftB(), key, node));
    else
        node->setRight(addNode(node->rightB(), key, node));

    return rebalance(node);
}

void BTree::add(int key) {
    root = addNode(dynamic_cast<BTreeNode*>(root), key);
}



BTreeNode* removeMin(BTreeNode* node) {
    if(!node->left())
        return node->rightB();

    node->setLeft(removeMin(node->leftB()));
    return rebalance(node);
}

BTreeNode* findMin(BTreeNode* node) {
    return node->leftB() ? findMin(node->leftB()) : node;
}

BTreeNode* remove(BTreeNode* node, int key){
    if(!node)
        return node;

    if(key < node->key)
        node->setLeft(remove(node->leftB(), key));
    else if(key > node->key)
        node->setRight(remove(node->rightB(), key));
    else {
        auto l = node->leftB();
        auto r = node->rightB();
        delete node;
        if(!r)
            return l;
        auto min = findMin(r);
        min->setRight(removeMin(r));
        min->setLeft(l);
        return rebalance(min);
    }

    return rebalance(node);
}

void BTree::remove(int key) {
    root = ::remove(dynamic_cast<BTreeNode*>(root), key);
    invalidateSelf();
}
