#include "leetcode.h"

#include <iostream>

int main() {
    //std::vector<int> result = LeetCode::Array::twoSum_1({7, 11, 2, 15}, 9);
    std::vector<int> result = LeetCode::Array::twoSum_2({7, 11, 2, 15}, 9);

    for (const auto& item : result)
        std::cout << item << std::endl;

    return 0;
}