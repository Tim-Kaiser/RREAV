# pragma once

#include "glad/gl.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


class Interface {

public:
    Interface();

    void update();
    void draw();

    int getWidth() const;
    int getHeight() const;
    bool running() const;
private:
    sf::Window window_;

    bool isRunning_;
    int width_;
    int height_;
};

