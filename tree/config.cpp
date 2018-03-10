//
// Created by egor9814 on 08.03.18.
//

#include "config.hpp"

Config *defaultConfig() {
    auto cfg = new Config;

    cfg->binaryTree.color = 0xffffffff;
    cfg->binaryTree.colorLeft = 0xff00ff00;
    cfg->binaryTree.colorRight = 0xff0000ff;
    cfg->binaryTree.pathColor = 0xffffffff;
    cfg->binaryTree.pathColorLeft = 0xff00ff00;
    cfg->binaryTree.pathColorRight = 0xff0000ff;
    cfg->binaryTree.textColor = 0xffff0000;
    cfg->binaryTree.backgroundColor = 0xff292929;

    cfg->lastState.width = 800;
    cfg->lastState.height = 600;
    cfg->lastState.fullscreen = false;

    return cfg;
}