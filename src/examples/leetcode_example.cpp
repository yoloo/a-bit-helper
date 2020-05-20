#include "leetcode/leetcode.h"

#include <iostream>

int main() {
    std::vector<int> result = LeetCode::Array::twoSum_2({2, 7, 11, 15}, 9);

    for (const auto& item : result)
        std::cout << item << std::endl;

    return 0;
}