/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_RUNNER_H_
#define TASK_TASK_RUNNER_H_

#include <vector>
#include <functional>

#include "thread/worker_thread.h"

namespace runner {

class TaskRunner : public WorkerThread::Delegate  {
 public:
  enum class Type {
    SEQUENCE,
    CONCURRENT
  };

  virtual void PostTask(std::function<void()> task) {
    PostDelayTask(task, 0);
  }

  virtual void PostDelayTask(std::function<void()> task, uint64_t delay) = 0;

  virtual bool CheckTerminatedAllWorkers() = 0;
  virtual void StopRunner() = 0;

  virtual std::vector<uint64_t> WorkerIdLists() = 0;

 protected:
  TaskRunner() = default;
  virtual ~TaskRunner() = default;
};

}

#endif  // TASK_TASK_RUNNER_H_
