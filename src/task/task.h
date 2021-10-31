/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_H_
#define TASK_TASK_H_

#include <iostream>
#include "callback/callback.h"

namespace runner {

class Task {
 public:
  Task() = default;
  Task(Callback task, uint64_t delay)
    : task_(task), delay_(delay) {

  }

 private:
  uint64_t delay_;
  Callback task_;

};

}  // namespace runner

#endif  // TASK_TASK_H_