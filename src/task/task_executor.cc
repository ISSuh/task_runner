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
  : worker_(std::thread(&TaskExecutor::ExcuteWork, this)),
    id_(std::hash<std::thread::id>{}(worker_.get_id())),
    running_(true),
    delegate_(task_runner_proxy) {
}

TaskExecutor::~TaskExecutor() = default;

void TaskExecutor::Join() {
  LOG(LogLevel::TRACE) << __func__;
  if (worker_.joinable()) {
    worker_.join();
  }
}

uint64_t TaskExecutor::GetWokerId() const {
  return id_;
}

void TaskExecutor::ExcuteWork() {
  StartWorker();
  Work();
  TerminateWorker();
}

void TaskExecutor::StartWorker() {
  running_ = true;
  delegate_->OnStartWorker(id_);
}

void TaskExecutor::Work() {
  while (delegate_->CanWakeUp(id_)) {
    Task task = delegate_->NextTask();
    if (!task.callback) {
      continue;
    }

    delegate_->OnStartTask();
    task.callback();
    delegate_->OnDidFinishTask();
  }
}

void TaskExecutor::TerminateWorker() {
  delegate_->OnTerminateWorker(id_);
  running_ = false;
}

}  // namespace runner
