#include "worker_thread_queue.h"
#include <cstdio>
#include <future>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

int sum(int a, int b) {
  std::cout << "sum: " << a + b << std::endl;
  return a + b;
}

std::string concatenate(const std::string &s1, const std::string &s2) {
  std::cout << "concatenate: " << s1 + s2 << std::endl;
  return s1 + s2;
}

double multiply(double a, double b) {
  std::cout << "multiply: " << a * b << std::endl;
  return a * b;
}

int main(int argc, char *argv[]) {

  ph::WorkerThreadQueue queue;

  queue.start();

  std::vector<std::variant<std::future<int>, std::future<std::string>,
                           std::future<double>, std::future<void>>>
      futures;

  futures.push_back(queue.enqueue(sum, 1, 2));
  futures.push_back(queue.enqueue(concatenate, std::string("Hello, "),
                                  std::string("world!")));

  futures.push_back(queue.enqueue(
      []() { std::this_thread::sleep_for(std::chrono::seconds(2)); }));
  futures.push_back(queue.enqueue(multiply, 3.14, 2.0));
  futures.push_back(
      queue.enqueue([]() { std::cout << "Hello, World!2332" << std::endl; }));

  for (auto &future_variant : futures) {
    std::visit(
        [](auto &&fut) {
          if constexpr (std::is_same_v<
                            typename std::decay_t<decltype(fut.get())>, void>) {
            fut.get(); // Call get() for side effects (sync, exception)
            std::cout << "result (void)" << std::endl;
          } else {
            auto result = fut.get();
            using ValueType = typename std::decay_t<decltype(result)>;

            if constexpr (std::is_same_v<ValueType, int>) {
              std::cout << "result (int): " << result << std::endl;
            } else if constexpr (std::is_same_v<ValueType, std::string>) {
              std::cout << "result (string): " << result << std::endl;
            } else if constexpr (std::is_same_v<ValueType, double>) {
              std::cout << "result (double): " << result << std::endl;
            } else {
              // This case should ideally not be reached if all variant types
              // are handled.
              std::cout << "result (unknown type)" << std::endl;
            }
          }
        },
        future_variant);
  }

  queue.stop();

  return 0;
}