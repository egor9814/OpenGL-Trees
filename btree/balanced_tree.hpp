//
// Created by egor9814 on 10.03.18.
//

#ifndef OPENGL_TREES_BALANCED_TREE_HPP
#define OPENGL_TREES_BALANCED_TREE_HPP


#include "../tree/binary_tree.hpp"
#include "../tree/search_tree.hpp"

class BTreeNode : public BinaryTreeNode {
public:
    int balance = 0;

    BTreeNode(int key, BTreeNode *parent) : BinaryTreeNode(key, parent){}

    BTreeNode*& leftB() {
        return reinterpret_cast<BTreeNode *&>(BinaryTreeNode::left());
    }

    BTreeNode*& rightB() {
        return reinterpret_cast<BTreeNode *&>(BinaryTreeNode::right());
    }

    void recoverParents();
};


class BTree : public SearchTree {
public:
    using SearchTree::KeyType;
    using SearchTree::root;

    BTree();

    BTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator);

    BTree(unsigned long keyCount, int *keys);

    explicit BTree(std::vector<int> &keys);

    void add(KeyType key) override;

    void remove(KeyType key) override;
};


#endif //OPENGL_TREES_BALANCED_TREE_HPP
