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
    virtual void OnStartThread() = 0;
    virtual void OnFinishThread() = 0;

    virtual void OnStartTask() = 0;
    virtual void OnDidFinishTask() = 0;

    virtual bool CanRunning() = 0;
    virtual bool CanWakeUp() = 0;

    virtual Task NextTask() = 0;
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

  Delegate* delegate_;

 private:
  static void ExcuteWork(WorkerThread* worker_, Delegate* delegate) {
    delegate->OnStartThread();
    worker_->Work();
    delegate->OnFinishThread();
  }

  std::unique_ptr<std::thread> worker_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_
