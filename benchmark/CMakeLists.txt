cmake_minimum_required(VERSION 3.3.0)

add_executable(spsc_benchmark spsc_queue_benchmark.cpp)
target_link_libraries(spsc_benchmark benchmark benchmark_main)
