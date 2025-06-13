#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

namespace ph {
class WorkerThreadQueue {
  template <typename Fn, typename... Args>
  using Task = std::future<typename std::invoke_result<Fn, Args...>::type>;

public:
  WorkerThreadQueue();
  ~WorkerThreadQueue();

  void start();
  void stop();

  template <typename Fn, typename... Args>
  auto enqueue(Fn &&fn, Args &&...args) -> Task<Fn, Args...> {

    using return_type = typename std::invoke_result<Fn, Args...>::type;

    auto task_to_enqueue = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));

    std::future<return_type> res = task_to_enqueue->get_future();
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      tasks_.push([task_to_enqueue]() { (*task_to_enqueue)(); });
    }
    cv_.notify_one();
    return res;
  }

private:
  void workerThreadLoop();

private:
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  std::thread worker_thread_;
  std::atomic<bool> stop_{false};
};
}; // namespace ph