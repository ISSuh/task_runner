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

  void WiatForTerminateWorkers() {
    auto iter = executors_.begin();
    while (!executors_.empty()) {
      uint64_t id = iter->first;
      TaskExecutor* executor = iter->second.get();

      executor->Join();

      executors_.at(id).reset(nullptr);
      executors_.erase(id);
      ++iter;
    }
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
