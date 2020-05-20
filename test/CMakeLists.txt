include_directories(thirdparty/googletest/googletest/include)

add_executable(divider_test divider_test.cpp)
target_link_libraries(divider_test division gtest)

add_executable(leetcode_test leetcode_test.cpp)
target_link_libraries(leetcode_test leetcode gtest)