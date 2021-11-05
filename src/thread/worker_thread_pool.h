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
  WorkerThreadPool(size_t thread_num = 1)
    : thread_num_(thread_num_) {}

  ~WorkerThreadPool() {}

  void 

  void Join() {

  }

 private:
  size_t thread_num_;
  std::vector<WorkerThread> pool_;
  
};

}  // namespace runner

#endif  // THREAD_WORKER_THREAD_POOL_H_
