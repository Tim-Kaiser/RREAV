#pragma once

#include "glad/gl.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

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
