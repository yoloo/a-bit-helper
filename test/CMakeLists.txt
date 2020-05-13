cmake_minimum_required(VERSION 3.2)

project(test)

include_directories(${DIVISION_HEADERS_DIR})
include_directories(thirdparty/googletest/googletest/include)

set(SOURCE_FILES main.cpp divider_tests.cpp)

add_executable(divider_tests ${SOURCE_FILES})
target_link_libraries(divider_tests division gtest)

install(TARGETS divider_tests DESTINATION bin)
