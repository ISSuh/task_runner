/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef BASE_TIME_H_
#define BASE_TIME_H_

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace runner {

class Time {
 public:
  Time() = default;
  ~Time() = default;

  static std::string CurrentTimeToDateStr() {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm current_local_time = *std::localtime(&current_time);

    const auto ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count() % 1000000;

    std::ostringstream date_str_stream;

    date_str_stream << std::put_time(&current_local_time, "%Y-%m-%d %H:%M:%S");
    date_str_stream << '.' << std::setfill('0') << std::setw(6) << ns;

    return date_str_stream.str();
  }
};

}  // namespace runner

#endif  // BASE_TIME_H_