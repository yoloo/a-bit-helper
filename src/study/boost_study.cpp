#include <iostream>

#include <boost/timer.hpp>

int main() {
    boost::timer tm;

    std::cout << tm.elapsed() << std::endl;

    return 0;
}