#include <optional>

#include "../include/Interface.h"

Interface::Interface():
isRunning_(true)
{
    window_.create(sf::VideoMode({800, 600}), "Test");
    window_.setActive(true);

    width_ = window_.getSize().x;
    height_ = window_.getSize().y;
}

void Interface::update()
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

void Interface::draw()
{
    window_.display();
}

bool Interface::running() const
{
    return isRunning_;
}

int Interface::getWidth() const
{
    return width_;
}

int Interface::getHeight() const
{
    return height_;
}