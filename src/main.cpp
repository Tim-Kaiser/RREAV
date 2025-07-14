#include "Window.h"

int main()
{
    Window window;

    while (window.running())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.update();
        window.draw();
    }
}
