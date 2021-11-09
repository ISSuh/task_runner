/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_SEQUENCED_TASK_RUNNER_H_
#define TASK_SEQUENCED_TASK_RUNNER_H_

#include <vector>
#include <mutex>
#include <memory>

#include "task/task_runner.h"
#include "task/task_executor.h"
#include "task/task_queue.h"
#include "base/time.h"

namespace runner {

class TaskExecutor;
class TaskQueue;

class SequencedTaskRunner final : public TaskRunnerProxy {
 public:
  SequencedTaskRunner(const std::string& label);
  virtual ~SequencedTaskRunner();

  // TaksRunner
  void PostDelayTask(std::function<void()>, TimeTick delay) override;

  void StopRunner() override;
  bool CheckTerminatedAllWorkers() override;
  void WiatForTerminateWorkers() override;

  std::vector<uint64_t> WorkersIdLists() override;

  // WokerThread::Delegate
  void OnStartThread() override;
  void OnFinishThread() override;
  void OnStartTask() override;
  void OnDidFinishTask() override;

  bool CanRunning() override;

  Task NextTask() override;
  bool CanWakeUp() override;
  
 private:
  std::unique_ptr<TaskExecutor> executor_;
  TaskQueue queue_;
  bool running_;
  
  std::condition_variable cv_;
  std::mutex mutex_;
};

}  // namespace runner

#endif  // TASK_SEQUENCED_TASK_RUNNER_H_
