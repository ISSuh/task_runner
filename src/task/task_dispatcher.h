/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_DISPATCHER_H_
#define TASK_TASK_DISPATCHER_H_

#include <mutex>

#include "task/task.h"
#include "task/task_queue.h"

namespace runner {

class TaskDispatcher {
 public:
  class TaskDispatcherClient {
   public:

  };

  void PushTask(Task task) {
    // need lock

    task_queue_.push(task);
  }

  Task NextTask() {
    // need lock

    Task task = task_queue_.front();
    task_queue_.pop();

    return task;
  }

 private:
  TaskQueue task_queue_;
};

}  // namespace runner

#endif  // TASK_TASK_DISPATCHER_H_
