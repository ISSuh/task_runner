/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_SEQUENCED_TASK_RUNNER_H_
#define TASK_SEQUENCED_TASK_RUNNER_H_

#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "task/task_runner.h"
#include "task/task_executor.h"
#include "base/time.h"

namespace runner {

class TaskExecutor;
class TaskQueue;

class SequencedTaskRunner final : public TaskRunnerProxy {
 public:
  explicit SequencedTaskRunner(const std::string& label);
  virtual ~SequencedTaskRunner();

  // TaksRunner
  void PostDelayTask(std::function<void()>, TimeTick delay) override;

  // TaksRunnerProxy
  void StopRunner() override;
  void WiatForTerminateWorkers() override;

  std::vector<uint64_t> WorkersIdLists() override;
  bool IsRunning() override;

  // WokerThread::Delegate
  void OnStartWorker(uint64_t id) override;
  void OnTerminateWorker(uint64_t id) override;
  void OnStartTask() override;
  void OnDidFinishTask() override;

  Task NextTask() override;
  bool CanWakeUp(uint64_t id) override;

 private:
  std::unique_ptr<TaskExecutor> executor_;

  using TaskQueue = std::priority_queue<Task>;
  TaskQueue queue_;

  bool running_;

  std::condition_variable cv_;
  std::mutex mutex_;
};

}  // namespace runner

#endif  // TASK_SEQUENCED_TASK_RUNNER_H_
