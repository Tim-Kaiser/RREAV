#include "SFML/Window/Window.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "../include/rreav/OpenGL/glad/gl.h"

#include <optional>
#include <stdexcept>

#include "../include/rreav/Window.h"

Window::Window() : isRunning_(true), name_("RREAV") {
  window_.create(sf::VideoMode({800, 600}), name_, sf::Style::Default,
                 sf::State::Windowed);
  if (!window_.setActive(true)) {
    throw std::runtime_error("Error setting widow to active.");
  }

  gladLoadGL(sf::Context::getFunction);

  width_ = window_.getSize().x;
  height_ = window_.getSize().y;
}

Window::Window(std::string windowName, unsigned int windowWidth,
               unsigned int windowHeight)
    : isRunning_(true), name_(windowName) {
  window_.create(sf::VideoMode({windowWidth, windowHeight}), windowName,
                 sf::Style::Default, sf::State::Windowed);
  setWindowActive();
  gladLoadGL(sf::Context::getFunction);

  width_ = window_.getSize().x;
  height_ = window_.getSize().y;
}

Window::~Window() { window_.close(); }

void Window::update() { handleEvents(); }

void Window::setFullscreen() {
  // SFML currently doesnt properly support switching between windowed and
  // fullscreen at runtime. Might need a custom implementation to share OpenGL
  // state between windows.
  //  Create fullscreen window -> move OpenGL state over to new window -> set
  //  new window as active (?)
}
void Window::setWindowed(unsigned int windowWidth, unsigned int windowHeight) {
  window_.setSize({windowWidth, windowHeight});
}
void Window::setWindowedFullscreen() {
  window_.setPosition({-8, 0});
  window_.setSize({sf::VideoMode::getDesktopMode().size});
}

void Window::draw() { window_.display(); }

bool Window::running() const { return isRunning_; }

int Window::getWidth() const { return window_.getSize().x; }

int Window::getHeight() const { return window_.getSize().y; }

void Window::setWindowActive() {
  if (!window_.setActive(true)) {
    throw std::runtime_error("Error setting widow to active.");
  }
}

void Window::handleEvents() {
  while (const std::optional event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      // end the program
      isRunning_ = false;
    } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
      // adjust the viewport when the window is resized
      glViewport(0, 0, resized->size.x, resized->size.y);
      width_ = resized->size.x;
      height_ = resized->size.y;
    } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        isRunning_ = false;
      } else if (keyPressed->scancode == sf::Keyboard::Scancode::F) {
        setWindowedFullscreen();
      }
    }
  }
}
