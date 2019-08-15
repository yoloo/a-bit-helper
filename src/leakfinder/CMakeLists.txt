cmake_minimum_required(VERSION 3.2)

project(leakfinder C CXX)

set(SOURCE_FILES
        leakfinder.cpp
        allocation_info.h
        allocation_info.cpp
        )

#set(LIBRARY_OUTPUT_PATH ${DIVISIBLE_INSTALL_LIB_DIR})
add_library(leakfinder SHARED ${SOURCE_FILES})

target_link_libraries(leakfinder dl)

install(TARGETS leakfinder DESTINATION ${DIVISIBLE_INSTALL_LIB_DIR})