cmake_minimum_required(VERSION 3.2)
project(a-bit-helper)

set(BENCHMARK_ENABLE_TESTING off)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -fPIC")

set(INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thirdparty)