cmake_minimum_required(VERSION 3.2)
project(leakfinder-example)

set(C_LEAK_FINDER_SOURCE_FILES leakfinder_example.c)
add_executable(c_leakfinder_example ${C_LEAK_FINDER_SOURCE_FILES})
#[[target_link_libraries(c_leakfinder_example leakfinder)]]

#[[set(C_LEAK_FINDER_THREAD_SOURCE_FILES leakfinder_example_thread.c)
add_executable(c_leakfinder_example_thread ${C_LEAK_FINDER_THREAD_SOURCE_FILES})
target_link_libraries(c_leakfinder_example_thread leakfinder)]]

set(CPP_LEAK_FINDER_SOURCE_FILES leakfinder_example.cpp)
add_executable(cpp_leakfinder_example ${CPP_LEAK_FINDER_SOURCE_FILES})
#[[target_link_libraries(cpp_leakfinder_example leakfinder)]]

install(TARGETS c_leakfinder_example DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})
install(TARGETS cpp_leakfinder_example DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})