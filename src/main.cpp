#include <iostream>
#include "../include/Interface.h"

int main() {
    std::cout << "Hello World" << std::endl;

    Interface interface;

    while(interface.running())
    {
        interface.update();
        interface.draw();
    }
    
    return 0;
}
