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
    namespace LinkedList {
        struct ListNode {
            int val;
            ListNode* next;

            ListNode(int x) : val(x), next(nullptr) {}
        };

        //面试题-24. 反转链表
        namespace ReverseLinkedList {
            // a.双指针迭代
            ListNode* one(ListNode* head);

            // b.递归实现
            ListNode* two(ListNode* head);

            // c.暴力破解
            ListNode* three(ListNode* head);
        }

        //24. 两两交换链表相邻节点
        namespace SwapPairsLinkedList {
            // a.双指针迭代(相邻节点交换val)
            ListNode* one(ListNode* head);

            // b.双指针迭代(重构next)
            ListNode* two(ListNode* head);
        }

        //141. 判断链表是否有环
        namespace HasCycleLinkedList {
            // a.地址是否出现过
            bool one(ListNode* head);

            // b.快慢指针
            bool two(ListNode* head);
        }
    }
};

#endif //A_BIT_HELPER_LEET_CODE_H
