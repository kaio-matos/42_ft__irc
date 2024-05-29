#ifndef LOG_H
#define LOG_H
#include <ft_irc.hpp>

class Log {
public:
  Log(const std::string &funcName) {
    if (VERBOSE) {
      std::cout << "[" << funcName << "] ";
    }
  }

  template <class T> Log &operator<<(const T &v) {
    if (VERBOSE) {
      std::cout << v;
    }
    return *this;
  }

  ~Log() {
    if (VERBOSE) {
      std::cout << "[END]" << RESET << std::endl;
    }
  }
};

#define DebugLog Log(__FUNCTION__)
#endif
