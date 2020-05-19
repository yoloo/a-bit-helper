include_directories(thirdparty/googletest/googletest/include)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(divider_tests ${SOURCE_FILES})
target_link_libraries(divider_tests division gtest)
