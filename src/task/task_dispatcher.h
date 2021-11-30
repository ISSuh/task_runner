/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_DISPATCHER_H_
#define TASK_TASK_DISPATCHER_H_

#include <string>
#include <vector>
#include <map>

#include "task_manager.h"
#include "callback/callback.h"
#include "base/time.h"

namespace runner {

class TaskManager;

class TaskDispatcher {
 public:
  explicit TaskDispatcher(TaskManager* manager);
  ~TaskDispatcher();

  void PostTask(const std::string& task_runner_label,
                const TaskCallback& task_callback);

  void PostDelayTask(const std::string& task_runner_label,
                const TaskCallback& task_callback, TimeTick delay);

 private:
  TaskManager* manager_;
};

}  // namespace runner

#endif  // TASK_TASK_DISPATCHER_H_
