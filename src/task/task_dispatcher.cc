/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "task/task_dispatcher.h"
#include "base/logging.h"

namespace runner {

TaskDispatcher::TaskDispatcher() = default;

TaskDispatcher::~TaskDispatcher() = default;

void TaskDispatcher::PostTask(const std::string& task_runner_label,
              std::function<void()> task_callback) {

  PostDelayTask(task_runner_label, task_callback, TimeTick());
}

void TaskDispatcher::PostDelayTask(const std::string& task_runner_label,
              std::function<void()> task_callback, TimeTick delay) {

}
}  // namespace runner
