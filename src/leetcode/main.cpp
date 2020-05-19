#include "interview.h"

#include <iostream>

int main() {
    LeetCode leetcode;
    std::vector<int> result = leetcode.twoSum({7, 11, 2, 15}, 9);

    for (const auto& item : result)
        std::cout << item << std::endl;

    return 0;
}