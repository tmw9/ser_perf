
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


add_library(Serialization STATIC serialization.cpp)

target_link_libraries(Serialization "${TORCH_LIBRARIES}")

set_property(TARGET Serialization PROPERTY CXX_STANDARD 17)
