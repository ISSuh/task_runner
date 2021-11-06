/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "thread/worker_thread.h"

namespace runner {

WorkerThread::WorkerThread(Delegate* delegate)
  : delegate_(delegate),
    worker_(new std::thread(ExcuteWork, this, delegate_)),
    id_(std::hash<std::thread::id>{}(worker_->get_id())) {
}

WorkerThread::~WorkerThread() = default;

uint64_t WorkerThread::GetWokerId() const {
  return id_;
}

void WorkerThread::Join() {
  if (worker_->joinable()) {
    worker_->join();
  }
}

void WorkerThread::ExcuteWork(WorkerThread* worker_, Delegate* delegate) {
  delegate->OnStartThread();
  worker_->Work();
  delegate->OnFinishThread();
}

}  // namespace runner
