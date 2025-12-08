#include "../include/rreav.h"

#define chunkSize 2048

int main() {
  //===== INIT =====
  Window window;
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
  while (window.running()) {
    audioManager.update();

    mesh.render();
    window.update();
    window.draw();
  }

  return 0;
}
