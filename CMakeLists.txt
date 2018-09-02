cmake_minimum_required(VERSION 3.3.0)

set(PROJECT_NAME_STR lock_free_queue)
project(${PROJECT_NAME_STR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)

link_libraries(profiler)

include_directories(include)
add_subdirectory(test)
add_subdirectory(benchmark)
add_subdirectory(doc)
