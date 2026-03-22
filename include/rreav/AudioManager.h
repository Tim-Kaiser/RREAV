#pragma once

#include "OpenGL/opengl.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#ifndef RREAV_CHUNK_SIZE
#define RREAV_CHUNK_SIZE 2048
#endif

#define RREAV_FREQUENCY_SIZE RREAV_CHUNK_SIZE / 2 + 1

class AudioManager {
public:
  AudioManager(std::string filepath);
  void getSampleData();
  void getFrequencyData();

  void play();
  void pause();
  void setVolume(float volume);
  void bindAudioBuffer();
  void update();

private:
  void setNormValues();
  void setupAudioSSBO();

  sf::SoundBuffer m_soundBuffer;
  sf::Sound m_sound;

  static std::vector<float> m_samples;
  static std::vector<float> m_frequencies;

  GLuint m_ssbo_samples;
  GLuint m_ssbo_frequencies;
  int m_maxValue;
  int m_minValue;
};
