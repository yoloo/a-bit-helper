#include <iostream>

#include <asio.hpp>

int main() {
    asio::io_context io;

    asio::steady_timer tm(io, asio::chrono::seconds(5));
    tm.wait();

    std::cout << "Hello, standalone asio!" << std::endl;

    return 0;
}