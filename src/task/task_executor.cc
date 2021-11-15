/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/task_executor.h"
#include "task/task.h"
#include "base/logging.h"

namespace runner {

TaskExecutor::TaskExecutor(TaskRunnerProxy* task_runner_proxy)
  : worker_(new std::thread(&TaskExecutor::ExcuteWork, this)),
    id_(std::hash<std::thread::id>{}(worker_->get_id())),
    running_(true),
    delegate_(task_runner_proxy) {
}

TaskExecutor::~TaskExecutor() = default;

void TaskExecutor::Join() {
  LOG(LogLevel::TRACE) << __func__;
  if (!running_.load()) {
    return;
  }

  if (worker_->joinable()) {
    worker_->join();
  }

  worker_.reset();
}

uint64_t TaskExecutor::GetWokerId() const {
  return id_;
}

void TaskExecutor::ExcuteWork() {
  LOG(LogLevel::INFO) << __func__;
  StartWorker();
  Work();
  TerminateWorker();
}

void TaskExecutor::StartWorker() {
  running_.store(true);
  delegate_->OnStartWorker();
}

void TaskExecutor::Work() {
  while (delegate_->CanRunning() && delegate_->CanWakeUp()) {
    Task task = delegate_->NextTask();

    delegate_->OnStartTask();
    task.callback();
    delegate_->OnDidFinishTask();
  }
}

void TaskExecutor::TerminateWorker() {
  delegate_->OnTerminateWorker();
  running_.store(false);
}

}  // namespace runner
