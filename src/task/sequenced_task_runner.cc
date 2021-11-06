/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/sequenced_task_runner.h"
#include "base/logging.h"

namespace runner {

SequencedTaskRunner::SequencedTaskRunner()
  : executor_(nullptr),
    running_(false) {
}

SequencedTaskRunner::~SequencedTaskRunner() {
  StopRunner();
}

void SequencedTaskRunner::PostDelayTask(std::function<void()> task_callback, uint64_t delay) {
  LOG(LogLevel::INFO) << __func__;
  Task task(task_callback, delay);
  queue_.push(task);

  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  LOG(LogLevel::INFO) << __func__;
  running_ = false;
  cv_.notify_all();
}

bool SequencedTaskRunner::CheckTerminatedAllWorkers() {
  LOG(LogLevel::INFO) << __func__;
  if (executor_ == nullptr) {
    return true;
  }  
  return false;
}

std::vector<uint64_t> SequencedTaskRunner::WorkerIdLists() {
  LOG(LogLevel::INFO) << __func__;
  if (executor_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return {executor_->GetWokerId()};
}

void SequencedTaskRunner::OnStartThread() {
  LOG(LogLevel::INFO) << __func__;
}

void SequencedTaskRunner::OnFinishThread() {
  LOG(LogLevel::INFO) << __func__;

  cv_.notify_all();
}

void SequencedTaskRunner::OnStartTask() {
  LOG(LogLevel::INFO) << __func__;
}

void SequencedTaskRunner::OnDidFinishTask() {
  LOG(LogLevel::INFO) << __func__;
}

bool SequencedTaskRunner::CanRunning() {
  LOG(LogLevel::INFO) << __func__;
  return running_;
}

Task SequencedTaskRunner::NextTask() {
  LOG(LogLevel::INFO) << __func__;
  return Task();
}

bool SequencedTaskRunner::CanWakeUp() {
  LOG(LogLevel::INFO) << __func__;

  std::unique_lock<std::mutex> lock(mutex_);

  cv_.wait(lock, [&](){
      return running_ && !queue_.empty();
  });

  lock.unlock();

  return true;
}

}  // namespace runner
