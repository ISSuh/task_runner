/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_SEQUENCED_TASK_RUNNER_H_
#define TASK_SEQUENCED_TASK_RUNNER_H_

#include <stddef.h>
#include <memory>

#include "task/task_queue.h"
#include "task/task_runner.h"
#include "task/task_executor.h"

namespace runner {

class SequencedTaskRunner : public TaskRunner {
 public:
  SequencedTaskRunner()
    : executor_(nullptr),
      running_(false) {}

  virtual ~SequencedTaskRunner() {}

  void PostDelayTask(std::function<void()>, uint64_t delay) override {

  }

  void OnStartWork() override {

  }

  void OnDidFinishWork() override {
    
  }
  
 private:
  std::unique_ptr<TaskExecutor> executor_;
  TaskQueue task_queue_;
  bool running_;
};

}  // namespace runner

#endif  // TASK_SEQUENCED_TASK_RUNNER_H_
