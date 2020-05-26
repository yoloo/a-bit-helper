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
                int len = nums.size();
                for (int i = 0; i < len; ++i) {
                    for (int j = i + 1; j < len; ++j) {
                        if (nums[i] + nums[j] == target)
                            return {i, j};
                    }
                }

                return {};
            }
        }
    }

    namespace LinkedList {
        namespace ReverseLinkedList {
            ListNode* one(ListNode* head) {
                if (nullptr == head || nullptr == head->next)
                    return head;

                ListNode* prev = nullptr;
                ListNode* cur  = head;
                ListNode* next = nullptr;
                while (cur != nullptr) {
                    next = cur->next;
                    cur->next = prev;
                    prev = cur;

                    cur = next;
                }

                return prev;
            }
        }
    }

}