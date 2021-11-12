/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/sequenced_task_runner.h"
#include "base/logging.h"

namespace runner {

SequencedTaskRunner::SequencedTaskRunner(const std::string& label)
  : TaskRunnerProxy(label),
    executor_(new TaskExecutor(dynamic_cast<TaskRunnerProxy*>(this))),
    running_(true) {
}

SequencedTaskRunner::~SequencedTaskRunner() {
  StopRunner();
}

void SequencedTaskRunner::PostDelayTask(std::function<void()> task_callback, TimeTick delay) {
  LOG(LogLevel::TRACE) << __func__;
  std::lock_guard<std::mutex> lock(mutex_);

  queue_.push(Task(task_callback, delay));
  cv_.notify_all();
}

void SequencedTaskRunner::StopRunner() {
  LOG(LogLevel::TRACE) << __func__;
  running_ = false;
  cv_.notify_all();
}

bool SequencedTaskRunner::CheckTerminatedAllWorkers() {
  LOG(LogLevel::TRACE) << __func__;
  if (executor_ == nullptr) {
    return true;
  }  
  return false;
}

void SequencedTaskRunner::WiatForTerminateWorkers() {
  LOG(LogLevel::TRACE) << __func__;
  if (executor_ == nullptr) {
    return;
  }
  executor_->Join();
}

std::vector<uint64_t> SequencedTaskRunner::WorkersIdLists() {
  LOG(LogLevel::TRACE) << __func__;
  if (executor_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return {executor_->GetWokerId()};
}

void SequencedTaskRunner::OnStartWorker() {
  LOG(LogLevel::TRACE) << __func__;
}

void SequencedTaskRunner::OnTerminateWorker() {
  LOG(LogLevel::TRACE) << __func__;
  cv_.notify_all();
}

void SequencedTaskRunner::OnStartTask() {
  LOG(LogLevel::TRACE) << __func__;
}

void SequencedTaskRunner::OnDidFinishTask() {
  LOG(LogLevel::TRACE) << __func__;
}

bool SequencedTaskRunner::CanRunning() {
  LOG(LogLevel::TRACE) << __func__;
  return running_;
}

Task SequencedTaskRunner::NextTask() {
  LOG(LogLevel::TRACE) << __func__;
  std::lock_guard<std::mutex> lock(mutex_);

  Task task = queue_.top();
  queue_.pop();

  return task;
}

bool SequencedTaskRunner::CanWakeUp() {
  LOG(LogLevel::TRACE) << __func__;

  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [&](){
        return running_ && !queue_.empty();
    });
    lock.unlock();
  }
  return true;
}

}  // namespace runner
