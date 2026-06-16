#define _USE_MATH_DEFINES

#include "../include/rreav/AudioManager.h"
#include "rreav/Config.h"
#include "rreav/kiss-fft/kiss_fft.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <numeric>
#include <vector>

std::vector<float> AudioManager::m_samples =
    std::vector<float>(Config::getInstance()->getChunkSize(), 0.0f);

std::vector<float> AudioManager::m_frequencies =
    std::vector<float>(Config::getInstance()->getFrequencySize(), 0.0f);

std::vector<float> AudioManager::m_hann_values =
    std::vector<float>(Config::getInstance()->getChunkSize(), 0.0f);

AudioManager::AudioManager(std::string filepath) : m_sound(m_soundBuffer) {
  if (!m_soundBuffer.loadFromFile(filepath)) {
    throw std::runtime_error("Error loading audio file.");
  }
  m_sound.setBuffer(m_soundBuffer);
  m_sound.setLooping(true);

  for (size_t i = 0; i < Config::getInstance()->getChunkSize(); i++) {
    float hannValue =
        0.5 *
        (1 - cos((2 * M_PI * i) / (Config::getInstance()->getChunkSize() - 1)));
    m_hann_values[i] = hannValue;
  }

  setupAudioSSBO();
};

void AudioManager::play() { m_sound.play(); }

void AudioManager::pause() { m_sound.pause(); }

void AudioManager::setVolume(float volume) { m_sound.setVolume(volume); }

void AudioManager::update() {

  getSampleData();
  getFrequencyData();

  std::vector<float> normSamples = normalizeSampleData();
  std::vector<float> normFrequencies = normalizeFrequencyData();

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_samples);
  GLvoid *p_samples = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  std::memcpy(p_samples, normSamples.data(),
              normSamples.size() * sizeof(float));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_frequencies);
  GLvoid *p_freq = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  std::memcpy(p_freq, normFrequencies.data(),
              normFrequencies.size() * sizeof(float));
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
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               Config::getInstance()->getChunkSize() * sizeof(float),
               m_samples.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo_samples);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  getFrequencyData();

  glGenBuffers(1, &m_ssbo_frequencies);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo_frequencies);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               Config::getInstance()->getFrequencySize() * sizeof(float),
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

  std::iota(m_samples.begin(), m_samples.end(), 0.0f);

  int64_t currentSamplePosition64 =
      static_cast<int64_t>(currentPlaytime.asSeconds() * sampleRate);
  size_t currentSamplePosition =
      currentSamplePosition64 < 0
          ? 0
          : static_cast<size_t>(currentSamplePosition64);

  if (currentSamplePosition >= sampleCount) {
    return;
  }

  const size_t chunkSize = Config::getInstance()->getChunkSize();
  const size_t halfChunk = chunkSize / 2;

  // Calculate the start position (half chunk before current position)
  size_t startPos = currentSamplePosition - halfChunk;
  startPos = std::max(startPos, (size_t)0);

  // Calculate the end position (half chunk after current position)
  size_t endPos = currentSamplePosition + halfChunk;
  endPos = std::min(endPos, sampleCount);

  // Copy samples from startPos to endPos
  size_t sampleIndex = 0;
  for (size_t i = startPos; i < endPos && sampleIndex < chunkSize; i++) {
    m_samples[sampleIndex] = static_cast<float>(samples[i]);
    sampleIndex++;
  }
};

void AudioManager::getFrequencyData() {
  if (m_samples.size() != Config::getInstance()->getChunkSize()) {
    return;
  }
  std::iota(m_frequencies.begin(), m_frequencies.end(), 0.0f);

  kiss_fft_cfg cfg =
      kiss_fft_alloc(Config::getInstance()->getChunkSize(), 0, NULL, NULL);
  if (!cfg) {
    return;
  }

  std::vector<kiss_fft_cpx> cx_in(Config::getInstance()->getChunkSize());
  std::vector<kiss_fft_cpx> cx_out(Config::getInstance()->getChunkSize());

  for (size_t i = 0; i < Config::getInstance()->getChunkSize(); i++) {
    cx_in[i].r = m_samples[i] * m_hann_values[i];
    cx_in[i].i = 0.0f;
  }

  kiss_fft(cfg, cx_in.data(), cx_out.data());
  free(cfg);

  for (size_t i = 0; i < Config::getInstance()->getFrequencySize(); i++) {
    m_frequencies[i] =
        std::sqrt(cx_out[i].r * cx_out[i].r + cx_out[i].i * cx_out[i].i);
  }
};

std::vector<float> AudioManager::normalizeSampleData() {
  std::vector<float> normalized;
  float range = static_cast<float>(m_maxValue - m_minValue);
  float denom = range != 0.0f ? range : 1.0f;

  std::transform(m_samples.begin(), m_samples.end(),
                 std::back_inserter(normalized), [this, denom](float sample) {
                   return (sample - static_cast<float>(m_minValue)) / denom;
                 });
  return normalized;
};

std::vector<float> AudioManager::normalizeFrequencyData() {
  std::vector<float> normalized;
  float range = *std::max_element(m_frequencies.begin(), m_frequencies.end()) -
                *std::min_element(m_frequencies.begin(), m_frequencies.end());
  float denom = range != 0.0f ? range : 1.0f;

  std::transform(m_frequencies.begin(), m_frequencies.end(),
                 std::back_inserter(normalized), [this, denom](float freq) {
                   return (freq - *std::min_element(m_frequencies.begin(),
                                                    m_frequencies.end())) /
                          denom;
                 });
  return normalized;
};