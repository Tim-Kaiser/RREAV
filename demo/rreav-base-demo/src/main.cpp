#include "../include/rreav.h"
#include "SFML/System/Clock.hpp"

int main() {
  //===== INIT =====
  Config cfg("resources/config.json");
  Window window(cfg.getWindowName(), 800, 600);
  ShaderManager shaderManager;
  AudioManager audioManager("resources/audio/hammock.ogg");

  std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders(
      "resources/shaders/main.vert", "resources/shaders/main.frag");
  glUseProgram(renderShader->m_shaderProgramID);

  Mesh mesh = loadObject("resources/objects/quad.obj");

  audioManager.setVolume(1.5f);
  audioManager.play();
  audioManager.bindAudioBuffer();

  sf::Clock clock;
  clock.start();
  while (window.running()) {
    int t = clock.getElapsedTime().asMilliseconds();
    // ~90fps
    if (t > 10 && t < 12) {
      audioManager.update();

      mesh.render();
      window.update();
      window.draw();

      clock.restart();
    }
  }

  return 0;
}
