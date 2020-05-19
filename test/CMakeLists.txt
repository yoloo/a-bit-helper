include_directories(thirdparty/googletest/googletest/include)

set(SOURCE_FILES 
    main.cpp 
    divider_tests.cpp
    )

add_executable(divider_tests ${SOURCE_FILES})

target_link_libraries(divider_tests division gtest)
