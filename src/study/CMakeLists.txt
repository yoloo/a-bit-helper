add_executable(as_study as_study.cpp)

include_directories(/usr/local/boost/include)
link_directories(/usr/local/boost/lib)
add_executable(boost_study boost_study.cpp)

include_directories(/usr/local/asio/include)
add_executable(asio_study asio_study.cpp)

add_executable(cpp11_study cpp11_study.cpp)

add_executable(template_study template_study.cpp)
