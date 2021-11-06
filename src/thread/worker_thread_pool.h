/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef THREAD_WORKER_THREAD_POOL_H_
#define THREAD_WORKER_THREAD_POOL_H_

#include <iostream>
#include <vector>

#include "thread/worker_thread.h"

namespace runner {

class WorkerThreadPool {
 public:
  WorkerThreadPool(std::vector<WorkerThread*> threads)
    : num_of_workers_(threads.size()),
      running_(false),
      pool_(threads) {}

  ~WorkerThreadPool() {
    Join();
  }

  void Start() {
    running_ = true;
    for (WorkerThread* worker : pool_) {
      worker->Start();
    }
  }

  void Join() {
    for (WorkerThread* worker : pool_) {
      worker->Join();
    }
  }

  bool IsRunning() const { return running_; }

 private:
  void RunWokers() {

  }

  size_t num_of_workers_;
  bool running_;
  std::vector<WorkerThread*> pool_;
};

}  // namespace runner

#endif  // THREAD_WORKER_THREAD_POOL_H_
