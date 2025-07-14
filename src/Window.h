# pragma once

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


class Window {

public:
    Window();

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


