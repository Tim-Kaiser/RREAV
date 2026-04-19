#include "../include/rreav/AudioManager.h"
#include "rreav/kiss-fft/kiss_fftr.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <vector>

std::vector<float> AudioManager::m_samples =
    std::vector<float>(RREAV_CHUNK_SIZE, 0.0f);

std::vector<float> AudioManager::m_frequencies =
    std::vector<float>(RREAV_FREQUENCY_SIZE, 0.0f);

AudioManager::AudioManager(std::string filepath) : m_sound(m_soundBuffer) {
  if (!m_soundBuffer.loadFromFile(filepath)) {
    throw std::runtime_error("Error loading audio file.");
  }
  m_sound.setBuffer(m_soundBuffer);
  m_sound.setLooping(true);

  setupAudioSSBO();
};

void AudioManager::play() { m_sound.play(); }

void AudioManager::pause() { m_sound.pause(); }

void AudioManager::setVolume(float volume) { m_sound.setVolume(volume); }

void AudioManager::update() {
  getSampleData();
  getFrequencyData();

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_samples);
  GLvoid *p_samples = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  std::memcpy(p_samples, m_samples.data(), m_samples.size() * sizeof(float));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_frequencies);
  GLvoid *p_freq = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  std::memcpy(p_freq, m_frequencies.data(),
              m_frequencies.size() * sizeof(float));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void AudioManager::setNormValues() {
  const int16_t *samples = m_soundBuffer.getSamples();
  const size_t sampleCount = m_soundBuffer.getSampleCount();

  if (sampleCount == 0) {
    m_maxValue = 0;
    m_minValue = 0;
    return;
  }

  m_maxValue = *std::max_element(samples, samples + sampleCount);
  m_minValue = *std::min_element(samples, samples + sampleCount);
}

void AudioManager::setupAudioSSBO() {
  setNormValues();

  glGenBuffers(1, &m_ssbo_samples);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_samples);
  glBufferData(GL_SHADER_STORAGE_BUFFER, RREAV_CHUNK_SIZE * sizeof(float),
               m_samples.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_samples);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  getFrequencyData();

  glGenBuffers(1, &m_ssbo_frequencies);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_frequencies);
  glBufferData(GL_SHADER_STORAGE_BUFFER, RREAV_FREQUENCY_SIZE * sizeof(float),
               m_frequencies.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo_frequencies);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void AudioManager::bindAudioBuffer() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_samples);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_frequencies);
}

void AudioManager::getSampleData() {
  sf::Time currentPlaytime = m_sound.getPlayingOffset();
  const size_t sampleRate = m_soundBuffer.getSampleRate();
  const size_t sampleCount = m_soundBuffer.getSampleCount();
  const int16_t *samples = m_soundBuffer.getSamples();

  if (sampleCount == 0) {
    std::fill(m_samples.begin(), m_samples.end(), 0.0f);
    return;
  }

  // Compensate for audio output latency by reading ahead
  float latencySeconds = RREAV_AUDIO_LATENCY_MS / 1000.0f;
  sf::Time compensatedTime = currentPlaytime + sf::seconds(latencySeconds);

  int64_t currentSamplePosition64 =
      static_cast<int64_t>(compensatedTime.asSeconds() * sampleRate);
  size_t currentSamplePosition =
      currentSamplePosition64 < 0
          ? 0
          : static_cast<size_t>(currentSamplePosition64);

  if (currentSamplePosition >= sampleCount) {
    currentSamplePosition %= sampleCount;
  }

  const size_t chunkSize =
      std::min(size_t(RREAV_CHUNK_SIZE), sampleCount - currentSamplePosition);
  const float range = static_cast<float>(m_maxValue - m_minValue);
  const float denom = range != 0.0f ? range : 1.0f;

  for (size_t i = 0; i < chunkSize; i++) {
    const int16_t sample = samples[currentSamplePosition + i];
    m_samples[i] = (static_cast<float>(sample) - m_minValue) / denom;
  }
};

void AudioManager::getFrequencyData() {
  if (m_samples.size() != RREAV_CHUNK_SIZE) {
    return;
  }

  kiss_fftr_cfg cfg = kiss_fftr_alloc(RREAV_CHUNK_SIZE, 0, NULL, NULL);
  if (!cfg) {
    return;
  }

  kiss_fft_scalar cx_in[RREAV_CHUNK_SIZE];
  kiss_fft_cpx cx_out[RREAV_FREQUENCY_SIZE];

  for (size_t i = 0; i < RREAV_CHUNK_SIZE; i++) {
    cx_in[i] = m_samples[i];
  }

  kiss_fftr(cfg, cx_in, cx_out);
  free(cfg);

  m_frequencies.resize(RREAV_FREQUENCY_SIZE);
  for (size_t i = 0; i < RREAV_FREQUENCY_SIZE; i++) {
    m_frequencies[i] = cx_out[i].r;
  }
};