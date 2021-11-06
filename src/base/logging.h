/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace runner {

enum LogLevel {
  ERROR = 0X01,
  WARN = 0X02,
  INFO = 0X03,
  DEBUG = 0X04,
  TRACE = 0X05,
};

void SetLogLevel(LogLevel level);
LogLevel GetLogLevel();

class LOG {
 public:
  LOG() = default;
  explicit LOG(LogLevel level);
  ~LOG();

  template <typename T>
  LOG& operator<<(const T& message);

 private:
  enum LogColorCode {
    RED      = 31,
    GREEN    = 32,
    YELLOW   = 33,
    BLUE     = 34,
    BLACK    = 39,
  };

  bool CanPrintLog();

  LogColorCode SelectLogColor(LogLevel level);
  std::string LogLevelToStr(LogLevel level);
  std::string DateToStr();

  LogLevel log_level_;
  std::ostringstream buffer_;
};

template <typename T>
LOG& LOG::operator<<(const T& message) {
  if (CanPrintLog()) {
    buffer_ << message;
  }
  return *this;
}

}  // namespace runner

#endif  // BASE_LOGGING_H_
