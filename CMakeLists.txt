cmake_minimum_required(VERSION 3.2)
project(study-a-bit)

#set(BENCHMARK_ENABLE_TESTING off)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC")

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thirdparty)