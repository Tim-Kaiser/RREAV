#pragma once

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include <cstdint>

class AudioManager {
public:
  AudioManager(std::string filepath, uint16_t chunkSize);
  void getSampleData(std::vector<float> &sampleVec);

  void play();
  void pause();
  void setVolume(float volume);

private:
  void setNormValues();

  sf::SoundBuffer m_soundBuffer;
  sf::Sound m_sound;
  uint16_t m_chunkSize;
  int m_maxValue;
  int m_minValue;
};