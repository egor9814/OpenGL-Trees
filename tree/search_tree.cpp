//
// Created by egor9814 on 08.03.18.
//

#include "search_tree.hpp"

SearchTree::SearchTree() : BinaryTree() {}

SearchTree::SearchTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator)
        : BinaryTree() {
    std::vector<int> keys;
    for (unsigned long i = 0; i < keyCount; i++) {
        keys.push_back(randomKeyGenerator(keys));
    }
    for (auto &i : keys) {
        add(i);
    }
}

SearchTree::SearchTree(unsigned long keyCount, int *keys) : BinaryTree() {
    for (unsigned long i = 0; i < keyCount; i++) {
        add(keys[i]);
    }
}

SearchTree::SearchTree(std::vector<int> &keys) : BinaryTree() {
    for (auto &i : keys) {
        add(i);
    }
}


bool SearchTree::isSearchTree() {
    return true;
}

void addKey(BinaryTreeNode *root, int key) {
    if (key < root->key) {
        if (root->left() == nullptr) {
            root->left() = new BinaryTreeNode(key, root);
        } else {
            addKey(root->left(), key);
        }
    } else {
        if (root->right() == nullptr) {
            root->right() = new BinaryTreeNode(key, root);
        } else {
            addKey(root->right(), key);
        }
    }
}

void SearchTree::add(int key) {
    if (getRoot() == nullptr) {
        setRoot(new BinaryTreeNode(key, nullptr));
    } else {
        ::addKey(getRoot(), key);
    }
}

BinaryTreeNode *SearchTree::removeNode(BinaryTreeNode *q) {
    BinaryTreeNode *s, *r, *d;
    if (q->right() == nullptr) {
        d = q;
        q = q->left();
        /*if(q != nullptr && q->left() != nullptr)
            q->left()->parent = q;*/
        if (q != nullptr)
            q->parent = d->parent;
        delete d;
        return q;
    }
    if (q->left() == nullptr) {
        d = q;
        q = q->right();
        /*if(q != nullptr && q->right() != nullptr)
            q->right()->parent = q;*/
        if (q != nullptr)
            q->parent = d->parent;
        delete d;
        return q;
    }
    s = q->right();
    if (s->left() == nullptr) {
        q->key = s->key;
        q->right() = s->right();
        if (q->right() != nullptr)
            q->right()->parent = q;
        delete s;
        return q;
    }
    r = nullptr;
    while (s->left() != nullptr) {
        r = s;
        s = s->left();
    }
    q->key = s->key;
    if (r != nullptr) {
        r->left() = s->right();
        if (r->left() != nullptr)
            r->left()->parent = r;
    }
    delete s;
    return q;
}

void SearchTree::remove(int key) {
    auto q = findKey(key);
    if (q != nullptr) {
        auto parent = q->parent;
        bool toLeft = parent != nullptr && parent->left() == q;
        bool toRight = parent != nullptr && parent->right() == q;
        auto removed = removeNode(q);
        if (toLeft) {
            parent->left() = removed;
        } else if (toRight) {
            parent->right() = removed;
        } else {
            setRoot(removed);
        }
    }
    invalidateSelf();
}

BinaryTreeNode *findKey(BinaryTreeNode *node, int &key) {
    if (node == nullptr)
        return nullptr;
    if (node->key == key) {
        return node;
    }
    if (key < node->key) {
        auto n = findKey(node->left(), key);
        if (n != nullptr)
            return n;
    } else {
        auto n = findKey(node->right(), key);
        if (n != nullptr)
            return n;
    }
    return nullptr;
}
BinaryTreeNode *SearchTree::findKey(int key) {
    return ::findKey(getRoot(), key);
    //return BinaryTree::findKey(key);
}


BinaryTreeNode* findMin(BinaryTreeNode* node){
    return node->left() ? findMin(node->left()) : node;
}

BinaryTreeNode *SearchTree::findMin() {
    return ::findMin(root);
}
