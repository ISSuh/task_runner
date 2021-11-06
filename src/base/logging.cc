/**
 *
 *  Copyright:  Copyright (c) 2020, ISSuh
 *
 */

#include "base/logging.h"

namespace runner {

static LogLevel system_log_level = LogLevel::INFO;

void SetLogLevel(LogLevel level) {
  system_log_level = level;
}

LogLevel GetLogLevel() {
  return system_log_level;
}

LOG::LOG(LogLevel level) : log_level_(level) {
  if (CanPrintLog()) {
    buffer_ << "\033[" << SelectLogColor(level) << "m"
            << "[" << DateToStr() << "][" << LogLevelToStr(level) << "] : ";
  }
}

LOG::~LOG() {
  if (CanPrintLog()) {
    buffer_ << std::endl;
    std::cerr << buffer_.str();
  }
}

bool LOG::CanPrintLog() {
  return log_level_ <= system_log_level;
}

LOG::LogColorCode LOG::SelectLogColor(LogLevel level) {
  LogColorCode color;
  switch (level) {
  case LogLevel::ERROR:
    color = LogColorCode::RED;
    break;
  case LogLevel::WARN:
    color = LogColorCode::YELLOW;
    break;
  case LogLevel::INFO:
    color = LogColorCode::GREEN;
    break;
  case LogLevel::DEBUG:
    color = LogColorCode::BLUE;
    break;
  case LogLevel::TRACE:
    color = LogColorCode::BLACK;
    break;
  }
  return color;
}

std::string LOG::LogLevelToStr(LogLevel level) {
  std::string label("");
  switch (level) {
  case LogLevel::ERROR:
    label = "ERROR";
    break;
  case LogLevel::WARN:
    label = "WARN";
    break;
  case LogLevel::INFO:
    label = "INFO";
    break;
  case LogLevel::DEBUG:
    label = "DEBUG";
    break;
  case LogLevel::TRACE:
    label = "TRACE";
    break;
  }
  return label;
}

std::string LOG::DateToStr() {
  auto now = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm current_local_time = *std::localtime(&current_time);

  const auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count() % 1000000;

  std::ostringstream date_str_stream;

  date_str_stream << std::put_time(&current_local_time, "%Y-%m-%d %H:%M:%S");
  date_str_stream << '.' << std::setfill('0') << std::setw(6) << ns;

  return date_str_stream.str();
}

}  // namespace runner
