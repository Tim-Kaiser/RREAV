#include "../include/AudioManager.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

std::vector<float> AudioManager::m_samples = std::vector<float>();

AudioManager::AudioManager(std::string filepath, uint16_t chunkSize,
                           GLuint ssboIndex)
    : m_sound(m_soundBuffer), m_chunkSize(chunkSize) {
  if (!m_soundBuffer.loadFromFile(filepath)) {
    throw std::runtime_error("Error loading audio file.");
  }
  m_sound.setBuffer(m_soundBuffer);
  m_sound.setLooping(true);

  setNormValues();
  setupAudioSSBO(ssboIndex);
};

void AudioManager::play() { m_sound.play(); }

void AudioManager::pause() { m_sound.pause(); }

void AudioManager::setVolume(float volume) { m_sound.setVolume(volume); }

void AudioManager::update() {
  getSampleData();
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  GLvoid *p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(p, m_samples.data(), m_samples.size() * sizeof(float));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void AudioManager::setNormValues() {
  const int16_t *samples = m_soundBuffer.getSamples();
  const size_t sampleCount = m_soundBuffer.getSampleCount();

  m_maxValue = *std::max_element(samples, samples + sampleCount);
  m_minValue = *std::min_element(samples, samples + sampleCount);
}

void AudioManager::setupAudioSSBO(GLuint bufferIndex) {
  glGenBuffers(1, &m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, m_chunkSize * sizeof(float),
               m_samples.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndex, m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void AudioManager::bindAudioBuffer() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
}

void AudioManager::getSampleData() {
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
  m_samples.clear();
  std::transform(v.begin(), v.end(), std::back_inserter(m_samples),
                 [this](int x) {
                   return (float)(x - m_minValue) / (m_maxValue - m_minValue);
                 });
  m_samples.shrink_to_fit();
};
