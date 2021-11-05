/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_RUNNER_H_
#define TASK_TASK_RUNNER_H_

#include <functional>

#include "thread/worker_thread.h"

namespace runner {

class TaskRunner : WorkerThread::Delegate  {
 public:
  virtual void PostTask(std::function<void()> task) {
    PostDelayTask(task, 0);
  }

  virtual void PostDelayTask(std::function<void()> task, uint64_t delay) = 0;

 protected:
  TaskRunner() = default;
  virtual ~TaskRunner() = default;
};

}

#endif  // TASK_TASK_RUNNER_H_
