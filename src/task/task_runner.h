/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_RUNNER_H_
#define TASK_TASK_RUNNER_H_

#include <string>
#include <vector>
#include <functional>

#include "task/task_executor_delegate.h"
#include "base/time.h"
#include "callback/callback.h"

namespace runner {

class TaskRunner {
 public:
  enum class Type {
    SEQUENCE,
    CONCURRENT
  };

  virtual void PostTask(const TaskCallback& callback);
  virtual void PostDelayTask(const TaskCallback& callback, TimeTick delay) = 0;

 protected:
  TaskRunner();
  virtual ~TaskRunner();
};

class TaskRunnerProxy : public TaskRunner,
                        public TaskExecutorDelegate {
 public:
  explicit TaskRunnerProxy(const std::string& label);
  virtual ~TaskRunnerProxy();

  virtual void StopRunner() = 0;
  virtual void WiatForTerminateWorkers() = 0;
  virtual std::vector<uint64_t> WorkersIdLists() = 0;
  virtual bool IsRunning() = 0;

  std::string label() const;

 private:
  std::string label_;
};

}

#endif  // TASK_TASK_RUNNER_H_
