#pragma once

#include <map>
#include <string>

#define DEFAULT_WINDOW_NAME "RREAV"
#define DEFAULT_CHUNK_SIZE 1024
#define DEFAULT_FREQUENCY_SIZE DEFAULT_CHUNK_SIZE / 2 + 1

class Config {
public:
  static Config *getInstance();
  void init(const std::string filepath);

  int getChunkSize();
  int getFrequencySize();
  std::string getWindowName();

  std::string getValue(std::string key);

private:
  Config();

  static Config *m_instance;
  int m_chunkSize;
  int m_frequencySize;
  std::string m_windowName;
  std::map<std::string, std::string> m_data;
};