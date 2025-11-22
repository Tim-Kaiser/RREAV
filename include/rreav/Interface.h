#pragma once

#include "OpenGL/opengl.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

class Interface {

public:
  Interface();
  Interface(std::string windowName, unsigned int windowWidth,
            unsigned int windowHeight);
  ~Interface();

  void update();

  void draw();

  void setFullscreen();
  void setWindowed(unsigned int windowWidth, unsigned int windowHeight);
  void setWindowedFullscreen();

  int getWidth() const;
  int getHeight() const;
  bool running() const;

private:
  sf::Window window_;

  void setWindowActive();
  void handleEvents();

  bool isRunning_;
  int width_;
  int height_;
  std::string name_;
};
