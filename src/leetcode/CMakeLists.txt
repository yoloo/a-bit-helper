file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(leetcode ${SOURCE_FILES} interview.h interview.cpp)