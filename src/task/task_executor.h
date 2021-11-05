/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_H_
#define TASK_TASK_EXECUTOR_H_

#include "thread/worker_thread.h"
#include "task/task_runner.h"

namespace runner {

class TaskExecutor : public WorkerThread {
 public:
  TaskExecutor(TaskRunner* task_runner)
    : WorkerThread(task_runner).
      task_runner_(task_runner) {}

  ~TaskExecutor() {
  }

  void Work() override {

  }

 private:
  TaskRunner* task_runner_;
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_H_
