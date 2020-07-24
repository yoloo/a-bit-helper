include_directories(thirdparty/googletest/googletest/include)

add_executable(leetcode_test leetcode_test.cpp)
target_link_libraries(leetcode_test leetcode gtest)