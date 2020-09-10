#include "thread/thread_pool.h"

#include <vector>
#include <future>
#include <iostream>

int main() {
    ThreadPool pool(4);

    auto result1 = pool.enqueue([](int a, int b) { return a * b; }, 42, 2);
    std::cout << result1.get() << std::endl;

    auto result2 = pool.enqueue([](const std::string& str1, const std::string& str2) { return str1 + str2; }, "hello ", "world");
    std::cout << result2.get() << std::endl;

    std::vector<std::future<int>> results;
    for (size_t i = 0; i < 100; ++i) {
        results.emplace_back(pool.enqueue([](int a, int b) { return a + b; }, i, i + 1));
    }
    for (auto& result : results) {
        std::cout << result.get() << std::endl;
    }

    std::string name("you");
    pool.enqueue([name]{
       std::cout << "fuck " << name << std::endl;
    });

    return 0;
}