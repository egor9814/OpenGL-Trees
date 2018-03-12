//
// Created by egor9814 on 08.03.18.
//

#include <list>
#include "binary_tree.hpp"
#include <GLext/random.hpp>

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
void leftRootRight(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    if(node){
        leftRootRight(keys, node->left());
        keys.push_back(node->key);
        leftRootRight(keys, node->right());
    }
}

template<typename KeyType>
void rightRootLeft(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    if(node){
        leftRootRight(keys, node->right());
        keys.push_back(node->key);
        leftRootRight(keys, node->left());
    }
}

/**
 *        100
 *      /     \
 *   50        150
 *  /  \      /   \
 * 5    55 125     175
 * */
template<typename KeyType>
void byLevels(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    std::list<BinaryTreeNode*> levels;
    levels.push_back(node);
    while(!levels.empty()){
        auto &n = levels.front();
        if(n->left())
            levels.push_back(reinterpret_cast<BinaryTreeNode *&&>(n->left()));
        if(n->right())
            levels.push_back(reinterpret_cast<BinaryTreeNode *&&>(n->right()));
        keys.push_back(n->key);
        levels.pop_front();
    }
}

template<typename KeyType>
void leftRightRoot(std::vector<KeyType> &keys, BinaryTreeNode *node) {
    if (node == nullptr)
        return;
    if (node->left()) {
        leftRightRoot(keys, node->left());
    } else if (node->right()) {
        leftRightRoot(keys, node->right());
    }
    keys.push_back(node->key);
    if (node->parent && node->parent->right() != node) {
        leftRightRoot(keys, node->parent->right());
    }
}

template<typename KeyType>
inline void visit(std::vector<KeyType> &keys, BinaryTreeNode *node, BinaryTree::VisitType &type) {
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
            leftRootRight(keys, node);
            break;
        case BinaryTree::VisitType_RightRootLeft:
            rightRootLeft(keys, node);
            break;
        case BinaryTree::VisitType_ByLevels:
            byLevels(keys, node/*, noneKey*/);
            break;
        case BinaryTree::VisitType_LeftRightRoot:
            leftRightRoot(keys, node);
            break;
    }
}

std::vector<int> BinaryTree::visit(Node *node, VisitType type) {
    std::vector<BinaryTree::KeyType> keys;
    ::visit(keys, node, type);
    return keys;
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