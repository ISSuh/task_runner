/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef THREAD_WOERKER_THREAD_H_
#define THREAD_WOERKER_THREAD_H_

#include <thread>
#include <memory>
#include <atomic>

#include "task/task.h"

namespace runner {

class WorkerThread {
 public:
  class Delegate {
   public:
    virtual void OnStartWorker() = 0;
    virtual void OnTerminateWorker() = 0;

    virtual void OnStartTask() = 0;
    virtual void OnDidFinishTask() = 0;

    virtual bool CanRunning() = 0;
    virtual bool CanWakeUp() = 0;

    virtual Task NextTask() = 0;
  };

  virtual void Work() = 0;

  uint64_t GetWokerId() const;

  void StartWorker();
  void TerminateWorker();

  void Join();

 protected:
  WorkerThread(Delegate* delegate);
  virtual ~WorkerThread();

  Delegate* delegate_;

 private:
  static void ExcuteWork(WorkerThread* worker);

  std::unique_ptr<std::thread> worker_;
  uint64_t id_;
  std::atomic_bool running_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_
