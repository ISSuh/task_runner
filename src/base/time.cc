/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "base/time.h"

namespace runner {

const int64_t kNanosecondsPerMicrosecond = 1000;
const int64_t kMicrosecondsPerMillisecond = 1000;

int64_t Time::InNanosecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

int64_t Time::InMicrosecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

int64_t Time::InMillisecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t Time::InSecond() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

std::string Time::CurrentTimeToDateStr() {
  auto now = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm current_local_time = *std::localtime(&current_time);

  const auto ms = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count() % 1000000;

  std::ostringstream date_str_stream;

  date_str_stream << std::put_time(&current_local_time, "%Y-%m-%d %H:%M:%S");
  date_str_stream << '.' << std::setfill('0') << std::setw(6) << ms;

  return date_str_stream.str();
}

TimeTick::TimeTick()
  : tick_(0) {
}

TimeTick::TimeTick(int64_t us)
  : tick_(us) {
}
  
TimeTick::~TimeTick() = default;

}  // namespace runner
