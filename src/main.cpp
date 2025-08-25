#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "AudioManager.h"
#include "Interface.h"
#include "Mesh.h"
#include "ObjectLoader.h"
#include "ShaderManager.h"
#include "Utils.h"

#define chunkSize 2048

int main() {
  //===== INIT =====
  Interface interface;
  ShaderManager shaderManager;
  AudioManager audioManager("resources/audio/sine_wave_1000hz_44.1sr.wav",
                            chunkSize);

  std::unique_ptr<Shader> renderShader = shaderManager.CreateShaders(
      "resources/shaders/main.vert", "resources/shaders/main.frag");
  glUseProgram(renderShader->m_shaderProgramID);

  Mesh mesh = loadObject("resources/objects/quad.obj");

  std::vector<float> samples;
  samples.reserve(chunkSize);

  audioManager.setVolume(0.02);
  audioManager.play();
  audioManager.getSampleData(samples);

  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, chunkSize * sizeof(float), &samples[0],
               GL_DYNAMIC_COPY);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

  while (interface.running()) {
    audioManager.getSampleData(samples);
    // std::cout << vectorToString(samples) << std::endl;

    mesh.render();
    interface.update();
    interface.draw();
  }

  return 0;
}
