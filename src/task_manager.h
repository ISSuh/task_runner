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

namespace runner {

class TaskManager {
 public:
  TaskManager() = default;
  ~TaskManager() {
    WaitForAllWorkerTerminate();

  }

  TaskRunner* CreateTaskRunner(const std::string& label, TaskRunner::Type type) {
    if (runner_map_.find(label) == runner_map_.end()) {
      return runner_map_.at(label);
    }
    
    TaskRunner* runner = nullptr;
    switch (type) {
    case TaskRunner::Type::SEQUENCE:
      runner = new SequencedTaskRunner();
      break;
    }

    runner_map_.insert({label, runner});
    return runner;
  }
  
  void StopAll() {
    for (auto runner : runner_map_) {
      runner.second->StopRunner();
    }
  }

  void TerminateRunner(const std::string& label) {
    WaitForWorkerTerminate();
    runner_map_.erase(label);
  }

 private:
  void WaitForWorkerTerminate() {
  }
 
  void WaitForAllWorkerTerminate() {
    
  }

  std::map<std::string, TaskRunner*> runner_map_;
};

}  // namespace runner

#endif  // TASK_MANAGER_H_
