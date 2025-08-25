#include "../include/AudioManager.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

AudioManager::AudioManager(std::string filepath, uint16_t chunkSize)
    : m_sound(m_soundBuffer), m_chunkSize(chunkSize) {
  if (!m_soundBuffer.loadFromFile(filepath)) {
    throw std::runtime_error("Error loading audio file.");
  }
  m_sound.setBuffer(m_soundBuffer);
  m_sound.setLooping(true);

  setNormValues();
};

void AudioManager::getSampleData(std::vector<float> &sampleVec) {
  sf::Time currentPlaytime = m_sound.getPlayingOffset();
  int64_t currentSamplePosition =
      currentPlaytime.asSeconds() * m_soundBuffer.getSampleRate();
  const size_t sampleCount = m_soundBuffer.getSampleCount();
  const int16_t *samples = m_soundBuffer.getSamples();

  size_t chunkSize = currentSamplePosition + m_chunkSize > sampleCount
                         ? sampleCount - currentSamplePosition
                         : m_chunkSize;
  std::vector<int> v;
  v.reserve(chunkSize);
  for (int i = 0; i < chunkSize; i++) {
    v.emplace_back(*(samples + i));
  }
  sampleVec.clear();
  std::transform(v.begin(), v.end(), std::back_inserter(sampleVec),
                 [this](int x) {
                   return (float)(x - m_minValue) / (m_maxValue - m_minValue);
                 });

  sampleVec.shrink_to_fit();
};

void AudioManager::play() { m_sound.play(); }

void AudioManager::pause() { m_sound.pause(); }

void AudioManager::setVolume(float volume) { m_sound.setVolume(volume); }

void AudioManager::setNormValues() {
  const int16_t *samples = m_soundBuffer.getSamples();
  const size_t sampleCount = m_soundBuffer.getSampleCount();

  m_maxValue = *std::max_element(samples, samples + sampleCount);
  m_minValue = *std::min_element(samples, samples + sampleCount);
}
