//
// Created by zhaoyongle on 2020/5/19.
//

#include "leetcode.h"

#include <unordered_map>

namespace LeetCode
{
    std::vector<int> Array::twoSum_1(const std::vector<int> &nums, int target)
    {
        std::unordered_map<int, int> anothers;
        for (int i = 0; i < nums.size(); ++i)
        {
            int another = target - nums[i];
            if (anothers.find(another) != anothers.end())
                return {anothers[another], i};

            anothers[nums[i]] = i;
        }

        return {};
    }
}