cmake_minimum_required(VERSION 3.10)
project(worker_thread)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd header 찾을 때

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} worker_thread_queue.h worker_thread_queue.cpp main.cpp)