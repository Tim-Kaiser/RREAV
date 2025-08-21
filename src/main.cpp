#include <glm/glm.hpp>

#include "../include/Interface.h"
#include "../include/Mesh.h"
#include "../include/ObjectLoader.h"
#include "../include/ShaderManager.h"

int main() {
  Interface interface;
  //===== SHADER INIT =====
  ShaderManager shaderManager;

  std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders(
      "resources/shaders/main.vert", "resources/shaders/main.frag");
  glUseProgram(renderShader->m_shaderProgramID);

  Mesh mesh = loadObject("resources/objects/quad.obj");

  while (interface.running()) {
    mesh.render();
    interface.update();
    interface.draw();
  }

  return 0;
}
