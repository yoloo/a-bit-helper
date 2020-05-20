include_directories(thirdparty/gbenchmark/include/benchmark)

add_executable(string_bench string_bench.cpp)
target_link_libraries(string_bench benchmark)

add_executable(memcpy_bench memcpy_bench.cpp)
target_link_libraries(memcpy_bench benchmark)

add_executable(memset_bench memset_bench.cpp)
target_link_libraries(memset_bench benchmark)

add_executable(array_bench array_bench.cpp)
target_link_libraries(array_bench benchmark)

add_executable(fibonacci_bench fibonacci_bench.cpp)
target_link_libraries(fibonacci_bench benchmark)

add_executable(leetcode_bench leetcode_bench.cpp)
target_link_libraries(leetcode_bench leetcode benchmark)