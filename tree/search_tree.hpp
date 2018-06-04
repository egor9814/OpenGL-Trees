//
// Created by egor9814 on 08.03.18.
//

#ifndef OPENGL_TREES_SEARCH_TREE_HPP
#define OPENGL_TREES_SEARCH_TREE_HPP

#include "binary_tree.hpp"

class SearchTree : public BinaryTree {
public:
    using BinaryTree::KeyType;

    SearchTree();

    SearchTree(unsigned long keyCount, const std::function<int(std::vector<int> &)> &randomKeyGenerator);

    SearchTree(unsigned long keyCount, int *keys);

    explicit SearchTree(std::vector<int> &keys);

    bool isSearchTree() override;

    void add(int key) override;

    void remove(int key) override;

    Node *findKey(int key) override;

    Node* findMin();

protected:
    BinaryTreeNode *removeNode(BinaryTreeNode *q);
};

#endif //OPENGL_TREES_SEARCH_TREE_HPP
