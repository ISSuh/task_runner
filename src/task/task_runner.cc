/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <vector>
#include <functional>

#include "task/task_runner.h"

namespace runner {

TaskRunner::TaskRunner() = default;

TaskRunner::~TaskRunner() = default;

void TaskRunner::PostTask(const TaskCallback& callback) {
  PostDelayTask(callback, TimeTick());
}

TaskRunnerProxy::TaskRunnerProxy(const std::string& label)
  : label_(label) {
}

TaskRunnerProxy::~TaskRunnerProxy() = default;

std::string TaskRunnerProxy::label() const {
  return label_;
}

}
