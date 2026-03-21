#include "../include/rreav.h"

int main() {
  //===== INIT =====
  Config cfg("resources/config.json");
  Window window(cfg.getWindowName(), 800, 600);
  ShaderManager shaderManager;
  AudioManager audioManager("resources/audio/sine_wave_1000hz_44.1sr.wav",
                            cfg.getChunkSize(), 0);

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
