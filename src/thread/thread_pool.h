/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef THREAD_THREAD_POOL_H_
#define THREAD_THREAD_POOL_H_

#include <iostream>
#include "thread/thread.h"

namespace runner {

class ThreadPool {
 public:
  ThreadPool(size_t thread_num = 1) : thread_num_(thread_num_) {}
  ~ThreadPool() {}

 private:
  size_t thread_num_;
  
};

}  // namespace runner

#endif  // THREAD_THREAD_POOL_H_