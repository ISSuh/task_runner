/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_H_
#define TASK_TASK_EXECUTOR_H_

#include <memory>
#include <thread>
#include <atomic>

#include "task/task_runner.h"
#include "task/task_executor_delegate.h"

namespace runner {

class TaskRunnerProxy;

class TaskExecutor {
 public:
  explicit TaskExecutor(TaskRunnerProxy* task_runner_proxy);
  virtual ~TaskExecutor();

  void Join();
  uint64_t GetWokerId() const;

 private:
  void ExcuteWork();

  void StartWorker();
  void Work();
  void TerminateWorker();

  std::thread worker_;
  uint64_t id_;
  bool running_;

  TaskExecutorDelegate* delegate_;
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_H_
