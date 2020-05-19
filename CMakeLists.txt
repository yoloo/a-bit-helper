cmake_minimum_required(VERSION 3.2)
project(study-a-bit)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC")

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(thirdparty)