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
#include "callback/callback.h"

namespace runner {

struct Task {
  Task() = default;
  Task(TaskCallback tasl_callback, TimeTick delay)
    : callback(tasl_callback), delay(delay) {}
  ~Task() = default;

  TaskCallback callback;
  TimeTick delay;

  bool operator<(const Task& s) const {
        return delay < s.delay;
  }
};

}  // namespace runner

#endif  // TASK_TASK_H_
