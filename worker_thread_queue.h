

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class WorkerThreadQueue {

public:
  WorkerThreadQueue();
  ~WorkerThreadQueue();

  void start();
  void stop();

  void enqueue(std::function<void()> task);

private:
  void workerThreadLoop();

private:
  std::queue<std::function<void()>> tasks_;
  std::thread worker_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  std::thread worker_thread_;
  std::atomic<bool> stop_{false};
};