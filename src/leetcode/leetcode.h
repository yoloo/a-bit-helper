//
// Created by zhaoyongle on 2020/5/19.
//

#ifndef A_BIT_HELPER_LEET_CODE_H
#define A_BIT_HELPER_LEET_CODE_H

#include <vector>

namespace LeetCode
{
    // 数组
    namespace Array
    {
        //1. 两数之和
        namespace TwoSum {
            //a.一次哈希
            std::vector<int> one(const std::vector<int>& nums, int target);

            //b.暴力破解
            std::vector<int> two(const std::vector<int>& nums, int target);
        }
    };

    // 链表
};

#endif //A_BIT_HELPER_LEET_CODE_H
