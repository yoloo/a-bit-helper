cmake_minimum_required(VERSION 3.2)

project(bench)

include_directories(thirdparty/gbenchmark/include/benchmark)

set(SOURCE_FILES string_bench.cpp)

add_executable(string_bench ${SOURCE_FILES})
target_link_libraries(string_bench benchmark)

install(TARGETS divider_tests DESTINATION bin)

