/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#include "thread/worker_thread.h"
#include "base/logging.h"

namespace runner {

WorkerThread::WorkerThread(Delegate* delegate)
  : delegate_(delegate),
    worker_(new std::thread(ExcuteWork, this)),
    id_(std::hash<std::thread::id>{}(worker_->get_id())),
    running_(false) {
}

WorkerThread::~WorkerThread() = default;

uint64_t WorkerThread::GetWokerId() const {
  return id_;
}

void WorkerThread::Join() {
  LOG(LogLevel::TRACE) << __func__;
  if (!running_.load()) {
    return;
  }

  if (worker_->joinable()) {
    worker_->join();
  }

  worker_.reset();
}

void WorkerThread::StartWorker() {
  running_.store(true);
  delegate_->OnStartWorker();
}

void WorkerThread::TerminateWorker() {
  delegate_->OnTerminateWorker();
  running_.store(false);
}

void WorkerThread::ExcuteWork(WorkerThread* worker) {
  LOG(LogLevel::TRACE) << __func__;
  worker->StartWorker();
  worker->Work();
  worker->TerminateWorker();
}

}  // namespace runner
