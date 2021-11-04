/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef THREAD_WOERKER_THREAD_H_
#define THREAD_WOERKER_THREAD_H_

#include <thread>
#include <memory>

namespace runner {

class WorkerThread {
 public:
  virtual void Work() = 0;

  void Start() {
  }

  void Join() { worker_->join(); }

 protected:
  WorkerThread() : worker_ {}
  virtual ~WorkerThread() = default;

 private:
  static void ExcuteWork(WorkerThread* worker_thraed) { worker_thraed->Work(); }

  std::unique_ptr<std::thread> worker_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_