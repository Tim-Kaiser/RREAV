#pragma once

#include <map>
#include <string>

class Config {
public:
  Config(std::string filepath);

  void readFile(std::string filepath);

  int getChunkSize();
  std::string getWindowName();

  std::string getValue(std::string key);

private:
  int m_chunkSize;
  std::string m_windowName;
  std::map<std::string, std::string> m_data;
};