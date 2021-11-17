/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_DELEGATE_H_
#define TASK_TASK_EXECUTOR_DELEGATE_H_

#include "task/task.h"

namespace runner {

class TaskExecutorDelegate {
 public:
  virtual void OnStartWorker(uint64_t id) = 0;
  virtual void OnTerminateWorker(uint64_t id) = 0;
  virtual void OnStartTask() = 0;
  virtual void OnDidFinishTask() = 0;

  virtual bool CanWakeUp(uint64_t id) = 0;
  virtual Task NextTask() = 0;
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_DELEGATE_H_
