cmake_minimum_required(VERSION 3.3.0)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

add_executable(spsc_test spsc_queue_test.cpp)
target_link_libraries(spsc_test ${GTEST_BOTH_LIBRARIES} pthread)

add_executable(spsc_test_tsan spsc_queue_test.cpp)
target_link_libraries(spsc_test_tsan ${GTEST_BOTH_LIBRARIES} pthread)
target_compile_options(spsc_test_tsan PUBLIC -fsanitize=thread)
set_target_properties(spsc_test_tsan PROPERTIES LINK_FLAGS "-fsanitize=thread")
