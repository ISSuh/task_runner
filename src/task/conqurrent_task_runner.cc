/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/conqurrent_task_runner.h"
#include "base/logging.h"

namespace runner {

ConcurrentTaskRunner::ConcurrentTaskRunner(const std::string& label, size_t num)
  : TaskRunnerProxy(label),
    executor_pool_(new TaskExecutorPool(this, num)),
    running_(true) {
}

ConcurrentTaskRunner::~ConcurrentTaskRunner() {
  StopRunner();
}

void ConcurrentTaskRunner::PostDelayTask(std::function<void()> task_callback, TimeTick delay) {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(Task(task_callback, delay));
  cv_.notify_all();
}

void ConcurrentTaskRunner::StopRunner() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);
  running_ = false;
  cv_.notify_all();
}

void ConcurrentTaskRunner::WiatForTerminateWorkers() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  if (executor_pool_ == nullptr) {
    return;
  }
  executor_pool_->WiatForTerminateWorkers();
}

std::vector<uint64_t> ConcurrentTaskRunner::WorkersIdLists() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  if (executor_pool_ == nullptr) {
    return std::vector<uint64_t>();
  }
  return executor_pool_->WorkersIdLists();
}

void ConcurrentTaskRunner::OnStartWorker() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
}

void ConcurrentTaskRunner::OnTerminateWorker() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
}

void ConcurrentTaskRunner::OnStartTask() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
}

void ConcurrentTaskRunner::OnDidFinishTask() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
}

bool ConcurrentTaskRunner::CanRunning() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  return running_;
}

Task ConcurrentTaskRunner::NextTask() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;
  std::lock_guard<std::mutex> lock(mutex_);

  Task task = queue_.top();
  queue_.pop();

  return task;
}

bool ConcurrentTaskRunner::CanWakeUp() {
  LOG(LogLevel::TRACE) << "[" << label() << "] " << __func__;

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
