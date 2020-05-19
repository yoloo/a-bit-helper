#include "leetcode.h"

#include <iostream>

int main() {
    LeetCode::Array arr;
    std::vector<int> result = arr.twoSum_1({7, 11, 2, 15}, 9);

    for (const auto& item : result)
        std::cout << item << std::endl;

    return 0;
}