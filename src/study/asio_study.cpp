#include <iostream>
#include <asio.hpp>

namespace SyncTimer {
    void test() {
        asio::io_context io;

        asio::steady_timer t(io, asio::chrono::seconds(5));
        t.wait();

        std::cout << "Hello, I'm sync timer" << std::endl;
    }
}

namespace AsyncTimer {
    void test() {
        asio::io_context io;

        asio::steady_timer t(io, asio::chrono::seconds(5));
        t.async_wait([](const asio::error_code& err) {
            std::cout << "Hello, I'm async timer" << std::endl;
        });

        io.run();
    }
}

int main() {
    //SyncTimer::test();
    AsyncTimer::test();

    return 0;
}