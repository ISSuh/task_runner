/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef THREAD_WOERKER_THREAD_H_
#define THREAD_WOERKER_THREAD_H_

#include <thread>
#include <memory>

#include "task/task.h"

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

  uint64_t GetWokerId() const;  
  void Join();

 protected:
  WorkerThread(Delegate* delegate);
  virtual ~WorkerThread();

  Delegate* delegate_;

 private:
  static void ExcuteWork(WorkerThread* worker_, Delegate* delegate);

  std::unique_ptr<std::thread> worker_;
  uint64_t id_;
};

}  // namespace runner

#endif  // THREAD_WOERKER_THREAD_H_
