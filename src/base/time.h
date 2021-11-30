/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef BASE_TIME_H_
#define BASE_TIME_H_

#include <stddef.h>
#include <string>

namespace runner {

class Time {
 public:
  static int64_t InNanosecond();
  static int64_t InMicrosecond();
  static int64_t InMillisecond();
  static int64_t InSecond();

  static std::string CurrentTimeToDateStr();
};

class TimeTick {
 public:
  static const int64_t kNanosecondsPerMicrosecond = 1000;
  static const int64_t kMicrosecondsPerMillisecond = 1000;

  TimeTick();
  explicit TimeTick(int64_t us);
  ~TimeTick();

  static TimeTick FromMilliseconds(int64_t ms) {
    return From(ms, kMicrosecondsPerMillisecond);
  }

  // should change type of tick_ to int64
  // need check std::numeric_limits<int64_t>::max() of std::numeric_limits<int64_t>::min()
  TimeTick operator+(TimeTick other) const {
    return TimeTick(tick_ + other.tick_);
  }

  TimeTick operator-(TimeTick other) const {
    return TimeTick(tick_ - other.tick_);
  }

  TimeTick& operator+=(TimeTick other) {
    return *this = (*this + other);
  }

  TimeTick& operator-=(TimeTick other) {
    return *this = (*this - other);
  }

  TimeTick operator-() const {
    return TimeTick(-tick_);
  }

  bool operator==(TimeTick other) const {
    return tick_ == other.tick_;
  }

  bool operator!=(TimeTick other) const {
    return tick_ != other.tick_;
  }

  bool operator<(TimeTick other) const {
    return tick_ < other.tick_;
  }

  bool operator<=(TimeTick other) const {
    return tick_ <= other.tick_;
  }

  bool operator>(TimeTick other) const {
    return tick_ > other.tick_;
  }

  bool operator>=(TimeTick other) const {
    return tick_ >= other.tick_;
  }

 private:
  static TimeTick From(int64_t value, int64_t offset) {
    return TimeTick(value * offset);
  }

  // microseconds
  int64_t tick_;
};


}  // namespace runner

#endif  // BASE_TIME_H_