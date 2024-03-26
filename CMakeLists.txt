cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(ser_perf)

find_package(Torch REQUIRED)
add_subdirectory(serialization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(ser_perf ser_perf.cpp)

target_link_libraries(ser_perf "${TORCH_LIBRARIES}" Serialization)

target_include_directories(ser_perf PUBLIC ${PROJECT_SOURCE_DIR}/serialization)

set_property(TARGET ser_perf PROPERTY CXX_STANDARD 17)
