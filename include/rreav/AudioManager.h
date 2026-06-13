#pragma once

#include "OpenGL/opengl.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class AudioManager {
public:
  AudioManager(std::string filepath);

  void play();
  void pause();
  void setVolume(float volume);
  void bindAudioBuffer();
  void update();

private:
  void getSampleData();
  void getFrequencyData();

  void setNormValues();
  void setupAudioSSBO();
  std::vector<float> normalizeSampleData();

  sf::SoundBuffer m_soundBuffer;
  sf::Sound m_sound;

  static std::vector<float> m_samples;
  static std::vector<float> m_frequencies;

  GLuint m_ssbo_samples;
  GLuint m_ssbo_frequencies;
  int m_maxValue;
  int m_minValue;
};
