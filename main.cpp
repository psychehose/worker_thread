#include "worker_thread_queue.h"
#include <iostream>
#include <thread>

int sum(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
  std::cout << "Hello, World!" << std::endl;

  WorkerThreadQueue queue;
  queue.start();

  queue.enqueue(std::bind(sum, 1, 2));

  queue.enqueue([]() { std::cout << "Hello, World2!" << std::endl; });
  queue.enqueue([]() { std::cout << "Hello, World3!" << std::endl; });
  queue.enqueue([]() { std::cout << "Hello, World4!" << std::endl; });

  return 0;
}