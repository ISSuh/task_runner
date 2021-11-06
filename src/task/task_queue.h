/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_QUEUE_H_
#define TASK_TASK_QUEUE_H_

#include <queue>

#include "task/task.h"
namespace runner {

class TaskQueue {
 public:
  TaskQueue() {}
  ~TaskQueue() {}

  void push(const Task& task) { queue_.push(task); }

  Task front() { return queue_.front(); }
  void pop() { queue_.pop(); }

  bool empty() { return queue_.empty(); }
  size_t size() { return queue_.size(); }

  void clear() {
    while (!empty()) {
      pop();
    }    
  }

 private:
  std::queue<Task> queue_;
};

}  // namespace runner

#endif  // TASK_TASK_QUEUE_H_
