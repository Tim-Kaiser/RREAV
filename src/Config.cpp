#include "../include/rreav/Config.h"
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
Config::Config(std::string filepath) {
  std::ifstream cfg_file(filepath);
  json data = json::parse(cfg_file);
  data["chunkSize"].get_to(m_chunkSize);
  data["windowName"].get_to(m_windowName);

  m_data = std::map<std::string, std::string>();
}

std::string Config::getValue(std::string key) { return ""; }

int Config::getChunkSize() { return m_chunkSize; }

std::string Config::getWindowName() { return m_windowName; }