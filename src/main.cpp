#include <glm/glm.hpp>

#include "AudioManager.h"
#include "Interface.h"
#include "Mesh.h"
#include "ObjectLoader.h"
#include "ShaderManager.h"

#define chunkSize 2048

int main() {
  //===== INIT =====
  Interface interface;
  ShaderManager shaderManager;
  AudioManager audioManager("resources/audio/sine_wave_1000hz_44.1sr.wav",
                            chunkSize, 0);

  std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders(
      "resources/shaders/main.vert", "resources/shaders/main.frag");
  glUseProgram(renderShader->m_shaderProgramID);

  Mesh mesh = loadObject("resources/objects/quad.obj");

  audioManager.setVolume(0.02);
  audioManager.play();
  audioManager.bindAudioBuffer();
  while (interface.running()) {
    audioManager.update();

    mesh.render();
    interface.update();
    interface.draw();
  }

  return 0;
}
