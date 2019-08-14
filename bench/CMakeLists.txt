cmake_minimum_required(VERSION 3.2)

project(bench)

include_directories(thirdparty/gbenchmark/include/benchmark)

set(STRING_BENCH_SOURCE_FILES string_bench.cpp)
add_executable(string_bench ${STRING_BENCH_SOURCE_FILES})
target_link_libraries(string_bench benchmark)

set(MEMCPY_BENCH_SOURCE_FILES memcpy_bench.cpp)
add_executable(memcpy_bench ${MEMCPY_BENCH_SOURCE_FILES})
target_link_libraries(memcpy_bench benchmark)

set(MEMSET_BENCH_SOURCE_FILES memset_bench.cpp)
add_executable(memset_bench ${MEMSET_BENCH_SOURCE_FILES})
target_link_libraries(memset_bench benchmark)

install(TARGETS string_bench DESTINATION bin)