/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_DISPATCHER_H_
#define TASK_TASK_DISPATCHER_H_

#include <vector>
#include <map>

#include "base/time.h"
#include "task/task_runner.h"

namespace runner {

class TaskDispatcher {
 public:
  TaskDispatcher();
  ~TaskDispatcher();

  void PostTask(const std::string& task_runner_label,
                std::function<void()> task_callback);

  void PostDelayTask(const std::string& task_runner_label,
                std::function<void()> task_callback, TimeTick delay);
};

}  // namespace runner

#endif  // TASK_TASK_DISPATCHER_H_
