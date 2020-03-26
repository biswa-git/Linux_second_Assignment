cmake_minimum_required (VERSION 2.8)
project (ComputationalGeometryAlgorithm)

include_directories(${CMAKE_SOURCE_DIR}/source/hpp)
include_directories(${CMAKE_SOURCE_DIR}/source/third_party/boost)
include_directories(${CMAKE_SOURCE_DIR}/source/third_party/eigen)

add_executable(CGA
    source/cpp/curve.cpp
    source/cpp/main.cpp
    source/cpp/point.cpp
    source/cpp/node.cpp
    source/cpp/mesh.cpp
)

target_link_libraries(CGA
   ${CMAKE_SOURCE_DIR}/source/third_party/boost/stage/lib/libboost_filesystem.a
)

install(TARGETS CGA DESTINATION ${CMAKE_SOURCE_DIR}/exe)
