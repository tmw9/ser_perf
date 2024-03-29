cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(ser_perf)

find_package(Torch REQUIRED)
add_subdirectory(serialization)
add_subdirectory(ipc)

set(CMAKE_CXX_FLAGS "-g -lrt")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(ser_perf ser_perf.cpp)
add_executable(ser_perf_worker ser_perf_worker.cpp)

target_link_libraries(ser_perf "${TORCH_LIBRARIES}" Serialization IPC)
target_link_libraries(ser_perf_worker "${TORCH_LIBRARIES}" Serialization IPC)
# target_link_libraries(ser_perf "${TORCH_LIBRARIES}" Serialization)

target_include_directories(ser_perf PUBLIC ${PROJECT_SOURCE_DIR}/serialization ${PROJECT_SOURCE_DIR}/ipc)
target_include_directories(ser_perf_worker PUBLIC ${PROJECT_SOURCE_DIR}/serialization ${PROJECT_SOURCE_DIR}/ipc)
# target_include_directories(ser_perf PUBLIC ${PROJECT_SOURCE_DIR}/serialization)

set_property(TARGET ser_perf PROPERTY CXX_STANDARD 17)
set_property(TARGET ser_perf_worker PROPERTY CXX_STANDARD 17)
