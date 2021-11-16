/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef TASK_TASK_EXECUTOR_POOL_H_
#define TASK_TASK_EXECUTOR_POOL_H_

#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "task/task_executor.h"
#include "base/logging.h"

namespace runner {

class TaskExecutorPool {
 public:
  TaskExecutorPool(TaskRunnerProxy* task_runner_proxy, size_t num)
    : number_of_executor_(num) {
    for (size_t i = 0 ; i < num ; ++i) {
      std::unique_ptr<TaskExecutor> executor(new TaskExecutor(task_runner_proxy));
      executors_.insert({executor->GetWokerId(), std::move(executor)});
    }
  }

  ~TaskExecutorPool() {}

  void StopWorkers() {
    for (auto& excutor : executors_) {
      excutor.second->Stop();
    }
  }

  void WiatForTerminateWorkers() {
    for (auto& excutor : executors_) {
      excutor.second->Join();
      excutor.second.reset(nullptr);
    }

    LOG(INFO) << __func__ << " - after join";
  }

  std::vector<uint64_t> WorkersIdLists() {
    std::vector<uint64_t> id_lists;
    for (auto& excutor : executors_) {
      id_lists.emplace_back(excutor.first);
    }
    return id_lists;
  }

 private:
  size_t number_of_executor_;
  std::map<uint64_t, std::unique_ptr<TaskExecutor>> executors_;
};

}  // namespace runner

#endif  // TASK_TASK_EXECUTOR_POOL_H_
