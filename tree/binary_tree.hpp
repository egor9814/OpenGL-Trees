//
// Created by egor9814 on 08.03.18.
//

#ifndef OPENGL_TREES_BINARY_TREE_HPP
#define OPENGL_TREES_BINARY_TREE_HPP

#include "tree.hpp"

#include <utility>

class BinaryTreeNode : public TreeNode<int, 2, BinaryTreeNode> {
public:
    BinaryTreeNode(int key, BinaryTreeNode *parent) : TreeNode(key, parent) {}

    virtual BinaryTreeNode *&left() {
        return child[0];
    }

    virtual BinaryTreeNode *&right() {
        return child[1];
    }

    void setLeft(BinaryTreeNode* node) {
        setChild(node, 0);
    }

    void setRight(BinaryTreeNode* node) {
        setChild(node, 1);
    }

};


class BinaryTree : public Tree<BinaryTreeNode> {
public:
    using Tree<BinaryTreeNode>::KeyType;

    BinaryTree() : Tree() {}

    BinaryTree(unsigned long keyCount, std::function<KeyType(std::vector<KeyType> &)> randomKeyGenerator)
            : Tree() {
        std::vector<KeyType> keys {};
        for (unsigned long i = 0; i < keyCount; i++) {
            keys.push_back(randomKeyGenerator(keys));
        }
        for (auto &i : keys) {
            add(i);
        }
    }

    BinaryTree(unsigned long keyCount, KeyType *keys)
            : Tree() {
        for (unsigned long i = 0; i < keyCount; i++) {
            add(keys[i]);
        }
    }

    explicit BinaryTree(std::vector<KeyType> &keys)
            : Tree() {
        for (auto &i : keys) {
            add(i);
        }
    }


    enum VisitType {
        VisitType_NONE = 0,

        VisitType_RootLeftRight,
        VisitType_RootRightLeft,
        VisitType_LeftRootRight,
        VisitType_RightRootLeft,
        VisitType_ByLevels,
        VisitType_LeftRightRoot,
    };

    std::vector<KeyType> visit(BinaryTreeNode *node, VisitType type = VisitType_NONE);

    std::vector<KeyType> visit(VisitType type = VisitType_NONE) {
        return visit(root, type);
    }

    virtual bool isSearchTree();


protected:
    int randomKey() override;

    void readAndAddKey();

public:
    bool onKeyDown(KeyEvent &event) override;

};

#endif //OPENGL_TREES_BINARY_TREE_HPP
