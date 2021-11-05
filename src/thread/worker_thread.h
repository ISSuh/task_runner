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
  class Delegate {
   public:
    virtual void OnStartWork() = 0;
    virtual void OnDidFinishWork() = 0;
  };

  virtual void Work() = 0;

  void Start() {
    worker_.reset(new std::thread(ExcuteWork));
  }

  void Join() {
    if (worker_->joinable()) {
      worker_->join();
    }
  }

 protected:
  WorkerThread(Delegate* delegate)
    : delegate_(delegate) {}
  virtual ~WorkerThread() = default;

 private:
  void ExcuteWork() {
    delegate_->OnStartWork();
    Work();
    delegate_->OnDidFinishWork();
  }

  std::unique_ptr<std::thread> worker_;
  Delegate* delegate_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_
