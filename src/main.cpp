#define GLAD_GL_IMPLEMENTATION
#include <../include/glad/gl.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "../include/Interface.h"
#include "../include/ShaderManager.h"
#include "../include/ObjectLoader.h"
#include "../include/Model.h"


int main() {
    std::cout << "Hello World" << std::endl;

    Interface interface;
    //===== SHADER INIT =====
	ShaderManager shaderManager;

	std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders("../shaders/main.vert", "../shaders/main.frag");

	Object obj;
	loadObject("Objects/quad.obj", obj);
	//Model quadModel(&obj, false);


    while(interface.running())
    {
        //quadModel.Render();
        interface.update();
        interface.draw();
    }
    
    return 0;
}
