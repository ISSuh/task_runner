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

// class Task {
//  public:
//   Task() = default;
//   Task(Callback task, uint64_t delay)
//     : task_(task), delay_(delay) {

//   }

//  private:
//   uint64_t delay_;
//   Callback task_;

// };

}  // namespace runner

#endif  // TASK_TASK_H_