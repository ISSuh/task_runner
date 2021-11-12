/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_MANAGER_H_
#define TASK_MANAGER_H_

#include <string>
#include <map>
#include <memory>

#include "task/task_runner.h"
#include "task/task_dispatcher.h"

namespace runner {

class TaskManager {
 public:
  TaskManager();
  ~TaskManager();

  TaskRunner* CreateTaskRunner(const std::string& label, TaskRunner::Type type);
  TaskDispatcher* CreateTaskDispatcher();

  TaskRunner* GetTaskRunner(const std::string& label);

  void WaitForTerminateTaskRunner(const std::string& task_runner_label);
  void WaitForTerminateAllTaskRunner();

  void StopRunner(const std::string& label);
  void StopAllRunner();

  uint32_t NumberOfTaskRunner() const;
  std::vector<std::string> TaskRunnerLabels() const;

 private:
  void EraseTaskRunner();

  std::map<std::string, std::unique_ptr<TaskRunnerProxy>> runner_map_;
};

}  // namespace runner

#endif  // TASK_MANAGER_H_
