//
// Created by egor9814 on 08.03.18.
//

#ifndef OPENGL_TREES_CONFIG_HPP
#define OPENGL_TREES_CONFIG_HPP

struct Config {
    struct {
        int color, colorLeft, colorRight,
                pathColor, pathColorLeft, pathColorRight,
                textColor,
                backgroundColor;
    } binaryTree;
    struct {
        int width, height;
        bool fullscreen;
    } lastState;
};

Config *defaultConfig();

//Config* readConfig(const char* fileName);
//void writeConfig(const char* fileName, Config* config);

#endif //OPENGL_TREES_CONFIG_HPP
