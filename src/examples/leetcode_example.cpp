#include "leetcode/leetcode.h"

#include <iostream>

int main() {
    std::vector<int> result = LeetCode::Array::TwoSum::one({2, 7, 11, 15}, 9);

    for (const auto& item : result)
        std::cout << item << std::endl;

    return 0;
}