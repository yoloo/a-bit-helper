#include <cmath>
#include <map>
#include <iostream>

int main() {
    std::cout << "0向上取整: " << std::ceil(0) << std::endl;
    std::cout << "0.1向上取整: " << std::ceil(0.1) << std::endl;

    std::cout << "0向下取整: " << std::floor(0) << std::endl;
    std::cout << "0.1向下取整: " << std::floor(0.1) << std::endl;

    std::map<uint32_t, uint32_t> tables = {{1, 1}, {2, 2}};
    if (tables[3] != 0)
    {
        std::cout << "Fit it!" << std::endl;
    }
    for (const auto& item : tables) {
        std::cout << "(" << item.first << "," << item.second << ")" << std::endl;
    }

    return 0;
}