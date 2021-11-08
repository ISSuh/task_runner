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

class TaskRunner {
 public:
  enum class Type {
    SEQUENCE,
    CONCURRENT
  };

  void PostTask(std::function<void()> task) { PostDelayTask(task, 0);  }
  virtual void PostDelayTask(std::function<void()> task, uint64_t delay) = 0;

 protected:
  TaskRunner() = default;
  virtual ~TaskRunner() = default;
};

class TaskRunnerProxy : public TaskRunner,
                        public WorkerThread::Delegate  {
 public:
  virtual bool CheckTerminatedAllWorkers() = 0;
  virtual void StopRunner() = 0;
  virtual void WiatForFinishWorkers() = 0;
  virtual std::vector<uint64_t> WorkersIdLists() = 0;

  std::string label() const { return label_; }

 protected:
  explicit TaskRunnerProxy(const std::string& label)
    : label_(label) {}

  virtual ~TaskRunnerProxy() {};

 private:
  std::string label_;
};

}

#endif  // TASK_TASK_RUNNER_H_
