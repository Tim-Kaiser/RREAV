#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "../include/Interface.h"
#include "../include/ShaderManager.h"
#include "../include/ObjectLoader.h"
#include "../include/Model.h"


int main() {
    Interface interface;
    //===== SHADER INIT =====
	ShaderManager shaderManager;

	std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders("resources/shaders/main.vert", "resources/shaders/main.frag");

	Object obj;
	if(!loadObject("resources/objects/quad.obj", obj))
    {
        std::cout << "Error creating object!" << std::endl;
        return 1;
    }

	Model quadModel(&obj, false);

    while(interface.running())
    {
        quadModel.Render();
        interface.update();
        interface.draw();
    }
    
    return 0;
}
