//
// Created by zhaoyongle on 2020/5/19.
//

#include "leetcode.h"

#include <unordered_map>

namespace LeetCode {
    namespace Array {
        namespace TwoSum {
            std::vector<int> one(const std::vector<int> &nums, int target) {
                std::unordered_map<int, int> lefts;
                for (int i = 0; i < nums.size(); ++i) {
                    int sub = target - nums[i];
                    if (lefts.find(sub) != lefts.end())
                        return {lefts[sub], i};

                    lefts[nums[i]] = i;
                }

                return {};
            }

            std::vector<int> two(const std::vector<int> &nums, int target) {
                for (int i = 0; i < nums.size(); ++i) {
                    for (int j = 0; j < i + 1; ++j) {
                        if (nums[i] + nums[j] == target)
                            return {i, j};
                    }
                }

                return {};
            }
        }
    }
}