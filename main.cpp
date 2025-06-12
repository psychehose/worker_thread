#include "worker_thread_queue.h"
#include <iostream>

int main(int argc, char *argv[]) {

  ph::WorkerThreadQueue queue;
  queue.start();
  queue.enqueue([]() { std::cout << "Hello, World2!" << std::endl; });
  queue.enqueue([]() { std::cout << "Hello, World3!" << std::endl; });
  queue.enqueue([]() { std::cout << "Hello, World4!" << std::endl; });
  queue.stop();

  return 0;
}