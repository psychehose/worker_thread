#include "worker_thread_queue.h"
#include <functional>
#include <iostream>
#include <mutex>

namespace ph {
WorkerThreadQueue::WorkerThreadQueue()
    : worker_thread_(&WorkerThreadQueue::workerThreadLoop, this) {}

WorkerThreadQueue::~WorkerThreadQueue() {
  stop_ = true;
  cv_.notify_all();
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

void WorkerThreadQueue::start() { stop_ = false; }

void WorkerThreadQueue::stop() { stop_ = true; }

void WorkerThreadQueue::workerThreadLoop() {

  while (true) {
    std::function<void()> task_to_execute;
    std::unique_lock<std::mutex> lock(queue_mutex_);
    cv_.wait(lock, [this] { return !tasks_.empty() || stop_; });
    if (stop_ && tasks_.empty())
      break;

    if (!tasks_.empty()) {
      task_to_execute = std::move(tasks_.front());
      tasks_.pop();
      lock.unlock();
    } else {
      continue;
    }

    if (task_to_execute) {
      task_to_execute();
    } else {
      std::cout << "No task to execute" << std::endl;
    }
  }
}
} // namespace ph
