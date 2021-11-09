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
  Task(std::function<void()> task, TimeTick delay)
    : delay(delay), run(task) {}
  ~Task() = default;

  TimeTick delay;
  std::function<void()> run;
};

}  // namespace runner

#endif  // TASK_TASK_H_
