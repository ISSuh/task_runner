/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_MANAGER_H_
#define TASK_MANAGER_H_

#include <string>
#include <map>

#include "task/task_runner.h"

namespace runner {

class TaskManager {
 public:
  TaskManager();
  ~TaskManager();

  TaskRunner* CreateTaskRunner(const std::string& label, TaskRunner::Type type);
  
  void StopRunner(const std::string& label);
  void StopAllRunner();
  void WaitForTerminateTaskRunner();

 private:
  std::map<std::string, TaskRunnerProxy*> runner_map_;
};

}  // namespace runner

#endif  // TASK_MANAGER_H_
