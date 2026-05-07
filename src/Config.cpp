#include "../include/rreav/Config.h"
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

Config *Config::m_instance = nullptr;

Config *Config::getInstance() {
  if (m_instance == nullptr) {
    m_instance = new Config();
  }
  return m_instance;
}

using json = nlohmann::json;
void Config::init(const std::string filepath) {
  std::ifstream cfg_file(filepath);
  json data = json::parse(cfg_file);
  if (data.contains("chunkSize") && data["chunkSize"].is_number()) {
    m_chunkSize = data["chunkSize"].get<int>();
    m_frequencySize = m_chunkSize / 2 + 1;
  }

  if (data.contains("windowName") && data["windowName"].is_string()) {
    m_windowName = data["windowName"].get<std::string>();
  }

  for (auto &elem : data.items()) {
    std::string key = elem.key().data();
    m_data[key] = elem.value().dump();
  }
}

Config::Config() {
  m_chunkSize = DEFAULT_CHUNK_SIZE;
  m_frequencySize = DEFAULT_FREQUENCY_SIZE;
  m_windowName = DEFAULT_WINDOW_NAME;
}

std::string Config::getValue(std::string key) { return m_data[key]; }

int Config::getChunkSize() { return m_chunkSize; }

int Config::getFrequencySize() { return m_frequencySize; }

std::string Config::getWindowName() { return m_windowName; }