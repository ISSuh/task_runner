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

namespace runner {

class TaskExecutor;
class TaskQueue;

class SequencedTaskRunner final : public TaskRunner {
 public:
  SequencedTaskRunner();
  virtual ~SequencedTaskRunner();

  // TaksRunner
  void PostDelayTask(std::function<void()>, uint64_t delay) override;
  bool CheckTerminatedAllWorkers() override;
  void StopRunner() override;
  std::vector<uint64_t> WorkerIdLists() override;

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
