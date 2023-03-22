cmake_minimum_required(VERSION 3.24)
project(message_bus)

set(CMAKE_CXX_STANDARD 17)

add_executable(message_bus main.cpp function_traits.h MessageBus.h)
