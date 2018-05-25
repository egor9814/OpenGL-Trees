//
// Created by egor9814 on 10.03.18.
//

#include "balanced_tree.hpp"

void BTreeNode::recoverParents() {
    if (leftB()) {
        leftB()->parent = this;
        leftB()->recoverParents();
    }
    if (rightB()) {
        rightB()->parent = this;
        rightB()->recoverParents();
    }
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


BTreeNode *addNode(BTreeNode *node, int key, bool &b, BTreeNode *parent = nullptr) {
    if (!node) {
        node = new BTreeNode(key, parent);
        b = true;
        return node;
    }

    if (node->key == key) {
        b = false;
        return node;
    }

    if (key < node->key) {
        node->leftB() = addNode(node->leftB(), key, b, node);
        if (!b)
            return node;
        if (node->balance == 1) {
            node->balance = 0;
            b = false;
            return node;
        }
        if (node->balance == 0) {
            node->balance = -1;
            return node;
        }
        if (node->balance == -1) {
            node->balance = -2;
            auto left = node->leftB();
            if (left->balance == -1) {
                node->leftB() = left->rightB();
                left->rightB() = node;
                node->balance = left->balance = 0;
                node = left;
            } else {
                auto right = left->rightB();
                left->rightB() = right->leftB();
                right->leftB() = left;
                node->leftB() = right->rightB();
                right->rightB() = node;
                switch (right->balance) {
                    case -1:
                        node->balance = 1;
                        left->balance = 0;
                        break;

                    case 1:
                        node->balance = 0;
                        left->balance = -1;
                        break;

                    case 0:
                        node->balance = left->balance = 0;
                        break;

                    default:
                        break;
                }
                right->balance = 0;
                node = right;
            }
            b = false;
            return node;
        }
    }
    node->rightB() = addNode(node->rightB(), key, b, node);
    if (!b)
        return node;
    if (node->balance == -1) {
        node->balance = 0;
        b = false;
        return node;
    }
    if (node->balance == 0) {
        node->balance = 1;
        return node;
    }
    if (node->balance == 1) {
        node->balance = 2;
    }
    auto right = node->rightB();
    if (right->balance == 1) {
        node->rightB() = right->leftB();
        right->leftB() = node;
        node->balance = right->balance = 0;
        node = right;
    } else {
        auto left = right->leftB();
        right->leftB() = left->rightB();
        left->rightB() = right;
        node->rightB() = left->leftB();
        left->leftB() = node;
        switch (left->balance) {
            case 1:
                node->balance = -1;
                right->balance = 0;
                break;

            case -1:
                node->balance = 0;
                right->balance = 1;
                break;

            case 0:
                node->balance = right->balance = 0;
                break;

            default:
                break;
        }
        left->balance = 0;
        node = left;
    }
    b = false;
    return node;
}

void BTree::add(int key) {
    bool balance;
    root = addNode(dynamic_cast<BTreeNode *>(root), key, balance);
    if (root)
        dynamic_cast<BTreeNode *>(root)->recoverParents();
    root->parent = nullptr;
}


BTreeNode *remove(BTreeNode *node, int key) {
    if (!node)
        return node;

    if (node->key == key) {
        if (!node->leftB())
            return node->rightB();
        if (!node->rightB())
            return node->leftB();

        auto &left = node->leftB();
        while (left->leftB() && left->rightB()) {
            left = left->leftB();
        }

        if (left->leftB() == nullptr) {
            left->leftB() = node->rightB();
        } else {
            left->rightB() = node->rightB();
        }

        return node->leftB();
    }
    node->leftB() = remove(node->leftB(), key);
    node->rightB() = remove(node->rightB(), key);
    return node;
}

void BTree::remove(int key) {
    root = ::remove(dynamic_cast<BTreeNode *>(root), key);
}
