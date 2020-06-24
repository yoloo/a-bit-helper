add_executable(assembly_study assembly_study.cpp)

include_directories(/usr/local/boost/include)
link_directories(/usr/local/boost/lib)
add_executable(boost_study boost_study.cpp)

include_directories(/usr/local/asio/include)
add_executable(asio_study asio_study.cpp)
