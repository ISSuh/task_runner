/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_RUNNER_H_
#define TASK_TASK_RUNNER_H_

#include <vector>
#include <functional>
#include "base/time.h"

#include "thread/worker_thread.h"

namespace runner {

class TaskRunner {
 public:
  enum class Type {
    SEQUENCE,
    CONCURRENT
  };

  virtual void PostTask(std::function<void()> task);
  virtual void PostDelayTask(std::function<void()> task, TimeTick delay) = 0;

 protected:
  TaskRunner();
  virtual ~TaskRunner();
};

class TaskRunnerProxy : public TaskRunner,
                        public WorkerThread::Delegate  {
 public:
  explicit TaskRunnerProxy(const std::string& label);
  virtual ~TaskRunnerProxy();

  virtual bool CheckTerminatedAllWorkers() = 0;
  virtual void StopRunner() = 0;
  virtual void WiatForTerminateWorkers() = 0;
  virtual std::vector<uint64_t> WorkersIdLists() = 0;

  std::string label() const;

 private:
  std::string label_;
};

}

#endif  // TASK_TASK_RUNNER_H_
