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

    BinaryTreeNode *&left() {
        return child[0];
    }

    BinaryTreeNode *&right() {
        return child[1];
    }

};


class BinaryTree : public Tree<BinaryTreeNode> {
public:
    using Tree<BinaryTreeNode>::KeyType;

    BinaryTree() : Tree() {}

    BinaryTree(unsigned long keyCount, std::function<KeyType(std::vector<KeyType> &)> randomKeyGenerator)
            : Tree(keyCount, std::move(randomKeyGenerator)) {}

    BinaryTree(unsigned long keyCount, KeyType *keys)
            : Tree(keyCount, keys) {}

    explicit BinaryTree(std::vector<KeyType> &keys)
            : Tree(keys) {}


    enum VisitType {
        VisitType_NONE = 0,

        VisitType_RootLeftRight,
        VisitType_RootRightLeft,
        VisitType_LeftRootRight,
        VisitType_RightRootLeft,
        VisitType_ByLevels,
        VisitType_LeftRightRoot,
    };

    std::vector<KeyType> visit(Node *node, KeyType noneKey, VisitType type = VisitType_NONE);

    std::vector<KeyType> visit(KeyType noneKey, VisitType type = VisitType_NONE) {
        return visit(root, noneKey, type);
    }

    virtual bool isSearchTree();


protected:
    int randomKey() override;

    void readAndAddKey();

public:
    bool onKeyDown(KeyEvent &event) override;

};

#endif //OPENGL_TREES_BINARY_TREE_HPP
