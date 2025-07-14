#include <optional>

#include "Window.h"

Window::Window():
window_(sf::VideoMode::getDesktopMode(), "Image Editor", sf::Style::Default, sf::State::Windowed, sf::ContextSettings(32)),
isRunning_(true)
{
    window_.setActive(true);

    width_ = window_.getSize().x;
    height_ = window_.getSize().y;
}

void Window::update()
{
    while (const std::optional event = window_.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            // end the program
            isRunning_ = false;
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, resized->size.x, resized->size.y);
            width_ = resized->size.x;
            height_ = resized->size.y;
        }
    }

}

void Window::draw()
{
    window_.display();
}

bool Window::running() const
{
    return isRunning_;
}

int Window::getWidth() const
{
    return width_;
}

int Window::getHeight() const
{
    return height_;
}
