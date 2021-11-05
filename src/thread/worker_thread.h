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

    virtual void WakeUp() = 0;
  };

  virtual void Work() = 0;

  void Start() {
    worker_.reset(new std::thread(ExcuteWork, this, delegate_));
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
  static void ExcuteWork(WorkerThread* worker_, Delegate* delegae) {
    delegae->OnStartWork();
    worker_->Work();
    delegae->OnDidFinishWork();
  }

  std::unique_ptr<std::thread> worker_;
  Delegate* delegate_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_
