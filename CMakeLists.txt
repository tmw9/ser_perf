cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(ser_perf)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(ser_perf ser_perf.cpp)
target_link_libraries(ser_perf "${TORCH_LIBRARIES}")
set_property(TARGET ser_perf PROPERTY CXX_STANDARD 17)