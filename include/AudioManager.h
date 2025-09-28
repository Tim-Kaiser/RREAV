#pragma once

#include "OpenGL/opengl.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cstdint>

class AudioManager {
public:
  AudioManager(std::string filepath, uint16_t chunkSize, GLuint ssboIndex);
  void getSampleData();

  void play();
  void pause();
  void setVolume(float volume);
  void bindAudioBuffer();
  void update();

private:
  void setNormValues();
  void setupAudioSSBO(GLuint bufferIndex);

  sf::SoundBuffer m_soundBuffer;
  sf::Sound m_sound;

  static std::vector<float> m_samples;
  GLuint m_ssbo;
  uint16_t m_chunkSize;
  int m_maxValue;
  int m_minValue;
};
