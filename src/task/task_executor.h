/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_H_
#define TASK_TASK_EXECUTOR_H_

#include "thread/worker_thread.h"
#include "task/task_runner.h"
#include "base/logging.h"

namespace runner {

class TaskExecutor : public WorkerThread {
 public:
  TaskExecutor(TaskRunnerProxy* task_runner_proxy)
    : WorkerThread(task_runner_proxy) {}

  virtual ~TaskExecutor() = default;

  void Work() override {
    while (delegate_->CanRunning() && delegate_->CanWakeUp()) {
      Task task = delegate_->NextTask();

      delegate_->OnStartTask();
      task.run();
      delegate_->OnDidFinishTask();
    }
  }
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_H_
