#pragma once
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

template <typename T> std::string vectorToString(const std::vector<T> &vec) {
  std::ostringstream oss;
  std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(oss, ","));
  oss << vec.back();

  return oss.str();
}