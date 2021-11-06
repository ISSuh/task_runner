/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_H_
#define TASK_TASK_EXECUTOR_H_

#include <memory>
#include <thread>
#include <condition_variable>

#include "thread/worker_thread.h"
#include "task/task_runner.h"
#include "task/task_dispatcher.h"

namespace runner {

class TaskExecutor : public WorkerThread {
 public:
  TaskExecutor(TaskRunner* task_runner)
    : WorkerThread(task_runner) {}

  virtual ~TaskExecutor() = default;

  void Work() override {
    while (delegate_->CanWakeUp() || delegate_->CanRunning()) {
      Task task = delegate_->NextTask();

      delegate_->OnStartTask();
      task.task();
      delegate_->OnDidFinishTask();
    }
  }
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_H_
