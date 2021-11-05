/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_SEQUENCED_TASK_RUNNER_H_
#define TASK_SEQUENCED_TASK_RUNNER_H_

#include <stddef.h>

#include "task/task_runner.h"

namespace runner {

class SequencedTaskRunner : public TaskRunner {
 public:
  void PostDelayTask(std::function<void()>, uint64_t delay) override {

  }

  void OnStartWork() override {

  }

  void OnDidFinishWork() override {
    
  }

 private:
  
};

}  // namespace runner

#endif  // TASK_SEQUENCED_TASK_RUNNER_H_
