/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_H_
#define TASK_TASK_H_

#include <stddef.h>
#include <functional>

#include "base/time.h"

namespace runner {

struct Task {
  Task(std::function<void()> callback, TimeTick delay)
    : delay(delay), callback(callback) {}
  ~Task() = default;

  TimeTick delay;
  std::function<void()> callback;

  bool operator<(const Task& s) const {
        return delay < s.delay;
  }
};

}  // namespace runner

#endif  // TASK_TASK_H_
