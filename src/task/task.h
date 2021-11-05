/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_H_
#define TASK_TASK_H_

#include <stddef.h>
#include <functional>

namespace runner {

struct Task {
  Task(std::function<void()> task, uint64_t delay)
    : delay(delay), task(task) {}

  uint64_t delay;
  std::function<void()> task;
};

}  // namespace runner

#endif  // TASK_TASK_H_
