/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/task_dispatcher.h"
#include "task/task_runner.h"
#include "base/logging.h"

namespace runner {

TaskDispatcher::TaskDispatcher(TaskManager* manager)
  : manager_(manager) {
}

TaskDispatcher::~TaskDispatcher() = default;

void TaskDispatcher::PostTask(const std::string& label,
              TaskCallback task_callback) {
  PostDelayTask(label, task_callback, TimeTick());
}

void TaskDispatcher::PostDelayTask(const std::string& label,
              TaskCallback task_callback, TimeTick delay) {
  TaskRunner* runner = manager_->GetTaskRunner(label);
  if (runner == nullptr) {
    return;
  }

  runner->PostDelayTask(task_callback, delay);
}

}  // namespace runner
