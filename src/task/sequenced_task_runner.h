/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_SEQUENCED_TASK_RUNNER_H_
#define TASK_SEQUENCED_TASK_RUNNER_H_

#include <mutex>
#include <memory>

#include "task/task_runner.h"
#include "task/task_executor.h"

namespace runner {

class SequencedTaskRunner : public TaskRunner {
 public:
  SequencedTaskRunner()
    : executor_(nullptr),
      running_(false) {
    }

  virtual ~SequencedTaskRunner() {
    running_ = false;
    cv_.notify_all();
  }

  void PostDelayTask(std::function<void()>, uint64_t delay) override {
    
    cv_.notify_all();
  }

  void OnStartThread() override {

  }

  void OnFinishThread() override {
    
  }

  void OnStartTask() override {

  }

  void OnDidFinishTask() override {
    
  }

  bool CanRunning() override {
    return running_;
  }

  bool CanWakeUp() override {
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [&](){
        return running_;
    });

    lock.unlock();

    return true;
  }

 private:
  std::unique_ptr<TaskExecutor> executor_;
  std::unique_ptr<TaskDispatcher> dispatcher_;
  
  bool running_;
  
  std::condition_variable cv_;
  std::mutex mutex_;
};

}  // namespace runner

#endif  // TASK_SEQUENCED_TASK_RUNNER_H_
