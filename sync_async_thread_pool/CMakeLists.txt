cmake_minimum_required(VERSION 3.24)
project(sync_async_thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(sync_async_thread_pool main.cpp sync_queue.h threadpool.h)
