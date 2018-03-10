//
// Created by egor9814 on 08.03.18.
//

#include "binary_tree.hpp"
#include "../glext/random.hpp"

bool isSearchTree(BinaryTreeNode *node) {
    auto l = node->left();
    auto r = node->right();
    if (l != nullptr && l->key >= node->key
        || r != nullptr && r->key <= node->key)
        return false;
    return !(l != nullptr && !isSearchTree(l) || r != nullptr && !isSearchTree(r));
}

bool BinaryTree::isSearchTree() {
    return ::isSearchTree(getRoot());
}


template<typename KeyType>
void rootLeftRight(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    if (node) {
        keys.push_back(node->key);
        rootLeftRight(keys, node->left());
        rootLeftRight(keys, node->right());
    }
}

template<typename KeyType>
void rootRightLeft(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    if (node) {
        keys.push_back(node->key);
        rootRightLeft(keys, node->right());
        rootRightLeft(keys, node->left());
    }
}

template<typename KeyType>
void leftRootRight(std::vector<KeyType> &keys, BinaryTreeNode *node, KeyType &noneKey,
                   unsigned long maxLevel,
                   unsigned long level = 1) {
    if (!node) {
        if (level <= maxLevel)
            keys.push_back(noneKey);
    } else {
        leftRootRight(keys, node->left(), noneKey, maxLevel, level + 1);
        keys.push_back(node->key);
        leftRootRight(keys, node->right(), noneKey, maxLevel, level + 1);
    }
}

template<typename KeyType>
void rightRootLeft(std::vector<KeyType> &keys, BinaryTreeNode *node, KeyType &noneKey,
                   unsigned long maxLevel,
                   unsigned long level = 1) {
    if (!node) {
        if (level <= maxLevel)
            keys.push_back(noneKey);
    } else {
        rightRootLeft(keys, node->right(), noneKey, maxLevel, level + 1);
        keys.push_back(node->key);
        rightRootLeft(keys, node->left(), noneKey, maxLevel, level + 1);
    }
}

template<typename KeyType>
void byLevels(std::vector<KeyType> &keys, BinaryTreeNode *node, KeyType &noneKey) {
    keys.push_back(node->key);
    for (auto i = 0UL; i < node->childCount; i++) {
        if (!node->child[i]) {
            keys.push_back(noneKey);
        } else {
            keys.push_back(node->child[i]->key);
        }
    }
    for (auto i = 0UL; i < node->childCount; i++) {
        if (node->child[i]) {
            byLevels(keys, node->child[i], noneKey);
        }
    }
}

template<typename KeyType>
void leftRightRoot(std::vector<KeyType> &keys, BinaryTreeNode *node, KeyType &noneKey) {
    if (node == nullptr)
        return;
    if (node->left()) {
        leftRightRoot(keys, node->left(), noneKey);
    } else if (node->right()) {
        leftRightRoot(keys, node->right(), noneKey);
    }
    keys.push_back(node->key);
    if (node->parent && node->parent->right() != node) {
        leftRightRoot(keys, node->parent->right(), noneKey);
    }
}

template<typename KeyType>
inline void visit(std::vector<KeyType> &keys, BinaryTreeNode *node, BinaryTree::VisitType &type,
                  KeyType &noneKey,
                  unsigned long height) {
    switch (type) {
        default:
            break;
        case BinaryTree::VisitType_RootLeftRight:
            rootLeftRight(keys, node);
            break;
        case BinaryTree::VisitType_RootRightLeft:
            rootRightLeft(keys, node);
            break;
        case BinaryTree::VisitType_LeftRootRight:
            leftRootRight(keys, node, noneKey, height);
            break;
        case BinaryTree::VisitType_RightRootLeft:
            rightRootLeft(keys, node, noneKey, height);
            break;
        case BinaryTree::VisitType_ByLevels:
            byLevels(keys, node, noneKey);
            break;
        case BinaryTree::VisitType_LeftRightRoot:
            leftRightRoot(keys, node, noneKey);
            break;
    }
}

std::vector<int> BinaryTree::visit(Node *node, VisitType type) {
    std::vector<BinaryTree::KeyType> keys;
    auto noneKey = getNoneKey();
    ::visit(keys, node, type, noneKey, findHeight());
    return keys;
}

int BinaryTree::getNoneKey() {
    return -1;
}


int BinaryTree::randomKey() {
    Random r;
    auto key = r.nextInt(1000);
    while (findKey(key) != nullptr) {
        key = r.nextInt(1000);
    }
    return key;
}


bool BinaryTree::onKeyDown(KeyEvent &event) {
    switch (event.getKey()) {
        default:
            return Tree::onKeyDown(event);
        case 'A':
            readAndAddKey();
            return true;
            /*case 'f':
                Dialog::text("Find node", "", "enter key",
                             [&](std::string res){
                                 try {
                                     auto key = std::stoi(res);
                                     findAndDraw(key);
                                 } catch(...){}
                                 invalidate();
                             },
                             [](){});
                return true;*/
    }
}


void BinaryTree::readAndAddKey() {
    /*Dialog::text("Add new node", "", "enter key",
                 [&](std::string res){
                     try {
                         auto key = std::stoi(res);
                         add(key);
                     } catch (...){}
                     invalidate();
                 },
                 [](){});*/
}