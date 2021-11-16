/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_CONCURRENT_TASK_RUNNER_H_
#define TASK_CONCURRENT_TASK_RUNNER_H_

#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "task/task_runner.h"
#include "task/task_executor_pool.h"
#include "base/time.h"

namespace runner {

class TaskExecutor;
class TaskQueue;

class ConcurrentTaskRunner final : public TaskRunnerProxy {
 public:
  explicit ConcurrentTaskRunner(const std::string& label, size_t num);
  virtual ~ConcurrentTaskRunner();

  // TaksRunner
  void PostDelayTask(std::function<void()>, TimeTick delay) override;

  // TaksRunnerProxy
  void StopRunner() override;
  void WiatForTerminateWorkers() override;

  std::vector<uint64_t> WorkersIdLists() override;
  bool IsRunning() override;

  // WokerThread::Delegate
  void OnStartWorker() override;
  void OnTerminateWorker() override;
  void OnStartTask() override;
  void OnDidFinishTask() override;

  bool CanRunning() override;

  Task NextTask() override;
  bool CanWakeUp() override;
  bool CanWakeUp(uint64_t id) override;

 private:
  std::unique_ptr<TaskExecutorPool> executor_pool_;

  using TaskQueue = std::priority_queue<Task>;
  TaskQueue queue_;

  bool running_;

  std::condition_variable cv_;
  std::mutex mutex_;
};

}  // namespace runner

#endif  // TASK_CONCURRENT_TASK_RUNNER_H_
