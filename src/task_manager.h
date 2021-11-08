/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_MANAGER_H_
#define TASK_MANAGER_H_

#include <string>
#include <map>

#include "task/task.h"
#include "task/sequenced_task_runner.h"
#include "base/logging.h"

namespace runner {

class TaskManager {
 public:
  TaskManager() = default;
  ~TaskManager() {
    StopAllRunner();
    WaitForFinishTaskRunner();
  }

  TaskRunner* CreateTaskRunner(const std::string& label, TaskRunner::Type type) {
    LOG(INFO) << __func__ << " - Create new Runner : " << label;

    if (runner_map_.find(label) != runner_map_.end()) {
      return runner_map_.at(label);
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

    runner_map_.insert({label, runner});
    return dynamic_cast<TaskRunner*>(runner);
  }
  
  void StopAllRunner() {
    for (auto runner : runner_map_) {
      runner.second->StopRunner();
    }
  }

  void StopRunner(const std::string& label) {
    if (runner_map_.find(label) == runner_map_.end()) {
      LOG(ERROR) << __func__ << " - Invalid label : " << label;
    }

    TaskRunnerProxy* runner = runner_map_.at(label);
    runner->StopRunner();
    runner_map_.erase(label);
  }

  void WaitForFinishTaskRunner() {
    for (const auto& runner : runner_map_) {
      runner.second->WiatForFinishWorkers();
    }
  }

 private:
  std::map<std::string, TaskRunnerProxy*> runner_map_;
};

}  // namespace runner

#endif  // TASK_MANAGER_H_
