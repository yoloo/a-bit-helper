cmake_minimum_required(VERSION 3.2)
project(division-example)

set(SOURCE_FILES division_example.cpp)
add_executable(divider_example ${SOURCE_FILES})
target_link_libraries(divider_example division)

install(TARGETS divider_example DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})