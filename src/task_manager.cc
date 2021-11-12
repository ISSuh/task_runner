/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include <string>
#include <map>

#include "task_manager.h"
#include "task/sequenced_task_runner.h"
#include "base/logging.h"

namespace runner {

TaskManager::TaskManager() = default;

TaskManager::~TaskManager() {
  StopAllRunner();
  WaitForTerminateAllTaskRunner();
}

TaskRunner* TaskManager::CreateTaskRunner(const std::string& label, TaskRunner::Type type) {
  LOG(INFO) << __func__ << " - Create new Runner : " << label;

  if (runner_map_.find(label) != runner_map_.end()) {
    return runner_map_.at(label).get();
  }
  
  TaskRunnerProxy* runner = nullptr;

  switch (type) {
    case TaskRunner::Type::SEQUENCE:
      runner = new SequencedTaskRunner(label);
      break;
    case TaskRunner::Type::CONCURRENT:
      LOG(LogLevel::ERROR) << __func__ << " - not implement CONCURRENT type";
      return nullptr;
  }

  runner_map_.insert({label, std::unique_ptr<TaskRunnerProxy>(std::move(runner))});
  return dynamic_cast<TaskRunner*>(runner);
}

void TaskManager::WaitForTerminateTaskRunner(const std::string& label) {
  if (runner_map_.find(label) == runner_map_.end()) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
  }

  TaskRunnerProxy* runner = runner_map_.at(label).get();
  runner->WiatForTerminateWorkers();

  // runner_map_.erase(label);
}

void TaskManager::WaitForTerminateAllTaskRunner() {
  for (const auto& runner : runner_map_) {
    const std::string label = runner.first;
    WaitForTerminateTaskRunner(label);
  }
}

void TaskManager::StopRunner(const std::string& label) {
  if (runner_map_.find(label) == runner_map_.end()) {
    LOG(ERROR) << __func__ << " - Invalid label : " << label;
  }

  TaskRunnerProxy* runner = runner_map_.at(label).get();
  runner->StopRunner();
}

void TaskManager::StopAllRunner() {
  for (const auto& runner : runner_map_) {
    const std::string label = runner.first;
    StopRunner(label);
  }
}

uint32_t TaskManager::NumberOfTaskRunner() const {
  return runner_map_.size();
}

std::vector<std::string> TaskManager::TaskRunnerLabels() const {
  std::vector<std::string> labels;
  for (const auto& runner : runner_map_) {
    const std::string label = runner.first;
    labels.emplace_back(label);
  }
  return labels;
}

}  // namespace runner
