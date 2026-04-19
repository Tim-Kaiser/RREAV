#include "../include/rreav.h"

int main() {
  //===== INIT =====
  Config cfg("resources/config.json");
  Window window(cfg.getWindowName(), 800, 600);
  ShaderManager shaderManager;
  AudioManager audioManager("resources/audio/pancake.ogg");

  std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders(
      "resources/shaders/main.vert", "resources/shaders/main.frag");
  glUseProgram(renderShader->m_shaderProgramID);

  Mesh mesh = loadObject("resources/objects/quad.obj");

  audioManager.setVolume(0.5f);
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
